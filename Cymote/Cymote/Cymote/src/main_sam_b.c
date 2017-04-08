/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/*
  Since we do not properly generate system init code, we define DONT_USE_CMSIS_INIT
  before including the device headers. We also comment out the call to SystemInit().
*/

/*
   EXT1  SAMB   LSM9DS0   WIRE    English
   ---------------------------------------
   SSC   15     CSXM      Red     Slave Select
   SCK   18     SCL       Black   Serial Clock
   MISO  17     SDOXM     Green   Master In Slave Out
   MOSI  16     SDA       Blue    Master Out Slave In
   VCC   20     VIN       Red
   GND   19     GND       Black
*/


#define DONT_USE_CMSIS_INIT
#include <asf.h>
#include "BLE.h"
#include "LSM9DS0.h"
#include "ADC_PWM.h"
#include "timer_hw.h"
#include "button.h"
#include "console_serial.h"

#define USE_BLE 1

/* Globals */
cymote_service_handler_t cymote_service_handler;
at_ble_handle_t cymote_connection_handle;
struct uart_module console_instance;
uint8_t dummy_data_counter = 0;
uint8_t dummy_data_counter_old = 0;
uint8_t dummy_data_2_state = 0;
uint8_t dummy_data[] = {'A', 'B', 'C', 'D', 'E', 'a'};
uint16_t accelerometer_data[3];
bool volatile flag = true;

/* timer callback function */
static void timer_callback_fn(void)
{
	/* Add timer callback functionality here */
	dummy_data_2_state = 1;

	//This breaks out of waiting for a BLE event
	send_plf_int_msg_ind(USER_TIMER_CALLBACK, TIMER_EXPIRED_CALLBACK_TYPE_DETECT, NULL, 0);
	DBG_LOG("timer callback\r\n");
}

static void button_cb(void)
{
	/* Add button callback functionality here */
	
	//sizeof(dummy_data) is the size of the array. If the counter is counting outside the array, reset it to 0
	if (dummy_data_counter >= (unsigned int)(sizeof(dummy_data)/sizeof(dummy_data[0])-1) ){
		dummy_data_counter = 0;
	}
	else dummy_data_counter++;
	
	dummy_data_2_state = 1;
	
	//This breaks out of waiting for a BLE event
	send_plf_int_msg_ind(USER_TIMER_CALLBACK, TIMER_EXPIRED_CALLBACK_TYPE_DETECT, NULL, 0);
	
	printf("Button CB\r\n");

}

 /* BLE Functions that require connection handles */

/* Callback registered for AT_BLE_PAIR_DONE event from stack */
at_ble_status_t ble_paired_app_event(void *param)
{
	at_ble_pair_done_t *at_ble_pair_done = (at_ble_pair_done_t *)param;
	LED_On(LED0);
	//hw_timer_start(BLE_UPDATE_INTERVAL);
	cymote_connection_handle = at_ble_pair_done->handle;
	return AT_BLE_SUCCESS;
}

static const ble_event_callback_t cymote_app_gap_cb[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	ble_connected_app_event,
	ble_disconnected_app_event,
	NULL,
	NULL,
	ble_paired_app_event,
	NULL,
	NULL,
	NULL,
	NULL,
	ble_paired_app_event,
	NULL,
	NULL,
	NULL,
	NULL
};


/* Callback registered for AT_BLE_NOTIFICATION_CONFIRMED event from stack */
static at_ble_status_t ble_notification_confirmed_app_event(void *param)
{
	at_ble_cmd_complete_event_t *notification_status = (at_ble_cmd_complete_event_t *)param;
	if(!notification_status->status)
	{
		flag = true;
		DBG_LOG_DEV("sending notification to the peer success");
	}
	return AT_BLE_SUCCESS;
}

at_ble_status_t cymote_char_changed_event(at_ble_handle_t conn_handle, cymote_service_handler_t *cymote_service, at_ble_characteristic_changed_t *char_handle, bool volatile *flag)
{
	at_ble_status_t status = AT_BLE_SUCCESS;
	at_ble_characteristic_changed_t change_params;
	memcpy((uint8_t *)&change_params, char_handle, sizeof(at_ble_characteristic_changed_t));
	
	if(cymote_service->service_characteristics[2].client_config_handle == change_params.char_handle)
	{
		if(change_params.char_new_value[0])
		{
			/* sending notification to the peer about change in the battery level */
			if((status = at_ble_notification_send(conn_handle, cymote_service->service_characteristics[2].char_val_handle)) != AT_BLE_SUCCESS) {
				DBG_LOG("sending notification failed%d",status);
				return status;
			}
			else {
				DBG_LOG_DEV("sending notification successful");
				*flag = false;
				return status;
			}
		}
		else
		{
			//do nothing
		}
	}
	return status;
}

/* Callback registered for AT_BLE_CHARACTERISTIC_CHANGED event from stack */
static at_ble_status_t ble_char_changed_app_event(void *param)
{
	at_ble_characteristic_changed_t *char_handle = (at_ble_characteristic_changed_t *)param;
	return cymote_char_changed_event(char_handle->conn_handle, &cymote_service_handler, char_handle, &flag);
}

static const ble_event_callback_t battery_app_gatt_server_cb[] = {
	ble_notification_confirmed_app_event,
	NULL,
	ble_char_changed_app_event,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

int main(void)
{
	at_ble_status_t status;
	cymote_info_data newData;
	uint16_t result;
	uint16_t result2;

    /* Initialize the SAM system */
	
	platform_driver_init();
	acquire_sleep_lock();
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

	/* Initialize serial console. */
	serial_console_init();
	//configure_console(&console_instance);
	
	/* Hardware timer. */
	hw_timer_init();
	hw_timer_register_callback(timer_callback_fn);
	//hw_timer_start(BLE_UPDATE_INTERVAL);
	
	/* Button initialization. */
	gpio_init();
	button_init();
	button_register_callback(button_cb);

	/* LSM9DS0 gpio configuration. */
	configure_gpio();

	/* Start up SPI and read the status register. */
	uint8_t receive = 0x00;
	bb_am_read_byte(WHO_AM_I_XM);
	receive = bb_am_read_byte(WHO_AM_I_XM);
	printf("Should be 0x49: 0x%x\r\n", receive);

	/* Initialize accelerometer control registers. */
	bb_init_accelerometer();
	bb_init_accelerometer_odr(A_ODR_100);
	bb_init_accelerometer_scale(A_SCALE_2G);
	
	/* Initialize magnetometer control registers. */
	init_magnetometer();
	init_magnetometer_odr(M_ODR_100);
	init_magnetometer_scale(M_SCALE_2GS);
	
	/* Initialize gyroscope control registers. */
	init_gyroscope();
	init_gyroscope_odr(G_ODR_190_BW_125);
	init_gyroscope_scale(G_SCALE_2000DPS);
	
	
	if(USE_BLE){
		DBG_LOG("Initializing BLE Application");
	
		/* Initialize the BLE chip and set the device address. */
		at_ble_addr_t addr;
		addr.type = AT_BLE_ADDRESS_PUBLIC;
		int i;
		for(i = 0; i < AT_BLE_ADDR_LEN; i++){
			addr.addr[i] = 0;
		}
		addr.addr[0] = 1;
		ble_device_init(&addr);

		cymote_init_service(&cymote_service_handler);
		if((status = cymote_primary_service_define(&cymote_service_handler)) != AT_BLE_SUCCESS){
			DBG_LOG("Device Information Service definition failed,reason %x",status);
		}

		device_information_advertise();

		/* Register callbacks for gap related events. */
		ble_mgr_events_callback_handler(REGISTER_CALL_BACK, BLE_GAP_EVENT_TYPE, cymote_app_gap_cb);
	}

	/* Initialize Joystick registers. */ 
	configure_adc_pin3();
	configure_adc_pin4();

    /* Main working loop. */
    while (1) 
    {
		if(USE_BLE){
			/* BLE Event task */
			ble_event_task(BLE_EVENT_TIMEOUT);
		
			/* Update BLE data */
			if (dummy_data_counter != dummy_data_counter_old){
				newData.data_len = 1;
				newData.info_data = dummy_data + dummy_data_counter;
				printf("data.info_data: %d\r\n", *(newData.info_data));
				UPDATE_DUMMY_DATA(&cymote_service_handler, &newData, cymote_connection_handle);
				dummy_data_counter_old = dummy_data_counter;
			}
			
			if (dummy_data_2_state != 0){
				dummy_data_2_state = 0;
			
				char newValue[DUMMY_DATA_2_MAX_LEN];
				int value = rand() % 1000;
				uint8_t len = snprintf(newValue, DUMMY_DATA_2_MAX_LEN, "%d", value);
				printf("newValue: %s\r\n", newValue);
			
				newData.info_data = (uint8_t*)newValue;
				newData.data_len = len;
				UPDATE_DUMMY_DATA_2(&cymote_service_handler, &newData, cymote_connection_handle);
			}
		}
		
		/*
		do {
		} while (adc_read(ADC_INPUT_CH_GPIO_MS1, &result) == STATUS_BUSY);
		do {
		} while (adc_read(ADC_INPUT_CH_GPIO_MS2, &result2) == STATUS_BUSY);
		
		configure_adc_pin3();
		configure_adc_pin4();
		//adc_read(ADC_INPUT_CH_GPIO_MS1, &result);
		//adc_read(ADC_INPUT_CH_GPIO_MS2, &result2);

		if (result > 1000)
		configure_pwm_from_duty_pin_10(99);
		else
		configure_pwm_from_duty_pin_10(result / 11);
		
		if (result2 > 1000)
		configure_pwm_from_duty_pin_11(99);
		else
		configure_pwm_from_duty_pin_11(result2 / 11);
		*/

		/* Print accelerometer data */
		bb_print_raw_accelerometer(accelerometer_data);

		
		char newValue[ACCEL_X_MAX_LEN];
		uint8_t value = accelerometer_data[0] >> 8;
		uint8_t len = snprintf(newValue, ACCEL_X_MAX_LEN, "%d", 123456);
		printf("Accel X High Byte: %s, len=%d\r\n", newValue, len);
		
		newData.info_data = (uint8_t*)newValue;
		newData.data_len = len;
		UPDATE_ACCEL_X_HIGH_DATA(&cymote_service_handler, &newData, cymote_connection_handle);

		
		value = (uint8_t)(accelerometer_data[0] & 0x00FF);
		len = snprintf(newValue, ACCEL_X_MAX_LEN, "%d", value);
		printf("Accel X Low Byte: %s\r\n", newValue);
		
		newData.info_data = (uint8_t*)newValue;
		newData.data_len = len;
		UPDATE_ACCEL_X_LOW_DATA(&cymote_service_handler, &newData, cymote_connection_handle);
		



		printf("aX: %d, aY: %d, aZ: %d\r\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);

    }
}











//int main(void)
//{
	//system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);
	//uint16_t result;
	//uint16_t result2;
	//configure_adc_pin3();
	//configure_adc_pin4();
//
	//while (true) {
		//do {
		//} while (adc_read(ADC_INPUT_CH_GPIO_MS1, &result) == STATUS_BUSY);
		//do {
		//} while (adc_read(ADC_INPUT_CH_GPIO_MS2, &result2) == STATUS_BUSY);
//
		//if (result > 1000)
		//configure_pwm_from_duty_pin_10(99);
		//else
		//configure_pwm_from_duty_pin_10(result / 11);
//
		//if (result2 > 1000)
		//configure_pwm_from_duty_pin_11(99);
		//else
		//configure_pwm_from_duty_pin_11(result2 / 11);
//
		//configure_adc_pin3();
		//configure_adc_pin4();
	//}
//}