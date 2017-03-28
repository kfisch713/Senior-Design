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
uint16_t accelerometer_data[3];
uint16_t gyroscope_data[3];
uint16_t magnetometer_data[3];
uint16_t joystick_data[2];
uint8_t  buttons;

/* timer callback function */
static void timer_callback_fn(void)
{
	/* Add timer callback functionality here */

	//This breaks out of waiting for a BLE event
	send_plf_int_msg_ind(USER_TIMER_CALLBACK, TIMER_EXPIRED_CALLBACK_TYPE_DETECT, NULL, 0);
	DBG_LOG("timer callback\r\n");
}

static void button_cb(void)
{
	/* Add button callback functionality here */
	
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

int main(void)
{
	at_ble_status_t status;
	cymote_info_data newDataX, newDataY, newDataZ;
	char newAccelX[ACCEL_MAX_LEN], newAccelY[ACCEL_MAX_LEN], newAccelZ[ACCEL_MAX_LEN];
	char newGyroX[GYRO_MAX_LEN], newGyroY[GYRO_MAX_LEN], newGyroZ[GYRO_MAX_LEN];
	char newMagnetX[MAGNET_MAX_LEN], newMagnetY[MAGNET_MAX_LEN], newMagnetZ[MAGNET_MAX_LEN];
	char newJoystickX[JOYSTICK_MAX_LEN], newJoystickY[JOYSTICK_MAX_LEN], newButtons[BUTTONS_MAX_LEN];
	at_ble_service_t cymote_service;
	cymote_characteristic_handle_t cymote_handles;

	uint16_t valueX, valueY, valueZ;
	uint8_t lenX, lenY, lenZ;


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
	hw_timer_start(BLE_UPDATE_INTERVAL);
	
	/* Button initialization. */
	gpio_init();
	button_init();
	button_register_callback(button_cb);

	/* LSM9DS0 gpio configuration. */
	configure_gpio();

	/* Start up SPI and read the status register. */
	uint8_t receive = 0x00;
	am_read_byte(WHO_AM_I_XM);
	receive = am_read_byte(WHO_AM_I_XM);
	printf("Should be 0x49: 0x%x\r\n", receive);

	/* Initialize accelerometer control registers. */
	init_accelerometer();
	init_accelerometer_odr(A_ODR_100);
	init_accelerometer_scale(A_SCALE_6G);
	
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
		addr.addr[0] = 3;
		ble_device_init(&addr);
		DBG_LOG("made it past ble device init");

		/*
		// old way
		cymote_init_service(&cymote_service_handler);
		if((status = cymote_primary_service_define(&cymote_service_handler, &chr_handle)) != AT_BLE_SUCCESS){
			DBG_LOG("Device Information Service definition failed,reason %x",status);
		}
		*/

		if((status = cymote_service_init(&cymote_service, &cymote_handles)) != AT_BLE_SUCCESS){
			DBG_LOG("Service definition failed,reason %x",status);
		}
		else {
			DBG_LOG("Service definition success");
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
			//get_raw_accelerometer(accelerometer_data);
			//get_raw_gyroscope(gyroscope_data);
			//get_raw_magnetometer(magnetometer_data);
			//do {
			//} while (adc_read(ADC_INPUT_CH_GPIO_MS1, &joystick_data[0]) == STATUS_BUSY);
			//do {
			//} while (adc_read(ADC_INPUT_CH_GPIO_MS2, &joystick_data[1]) == STATUS_BUSY);
			//get_button_data(buttons);

			accelerometer_data[0] = 10000;
			accelerometer_data[1] = 10001;
			accelerometer_data[2] = 10002;
			gyroscope_data[0] = 20000;
			gyroscope_data[1] = 20001;
			gyroscope_data[2] = 20002;
			magnetometer_data[0] = 30000;
			magnetometer_data[1] = 30001;
			magnetometer_data[2] = 30002;
			joystick_data[0] = 42;
			joystick_data[1] = 24;
			buttons = 2;
			
			at_ble_characteristic_value_set(cymote_handles.accel_x_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.accel_y_handle, "JK", 2);
			at_ble_characteristic_value_set(cymote_handles.accel_z_handle, "accelerometer_data[2]", 5);
			at_ble_characteristic_value_set(cymote_handles.gyro_x_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.gyro_y_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.gyro_z_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.magnet_x_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.magnet_y_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.magnet_z_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.joystick_x_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.joystick_y_handle, "lol", 3);
			at_ble_characteristic_value_set(cymote_handles.buttons_handle, "00000000", 8);

			/*
			//accelerometer
			valueX = accelerometer_data[0];
			valueY = accelerometer_data[1];
			valueZ = accelerometer_data[2];
			lenX = snprintf(newAccelX, ACCEL_MAX_LEN, "%d", valueX);
			lenY = snprintf(newAccelY, ACCEL_MAX_LEN, "%d", valueY);
			lenZ = snprintf(newAccelZ, ACCEL_MAX_LEN, "%d", valueZ);
			newDataX.info_data = (uint8_t*)newAccelX;
			newDataX.data_len = lenX;
			newDataY.info_data = (uint8_t*)newAccelY;
			newDataY.data_len = lenY;
			newDataZ.info_data = (uint8_t*)newAccelZ;
			newDataZ.data_len = lenZ;
			UPDATE_ACCEL(&cymote_service_handler, &newDataX, &newDataY, &newDataZ, cymote_connection_handle);
			
			//gyroscope
			valueX = gyroscope_data[0];
			valueY = gyroscope_data[1];
			valueZ = gyroscope_data[2];
			lenX = snprintf(newGyroX, GYRO_MAX_LEN, "%d", valueX);
			lenY = snprintf(newGyroY, GYRO_MAX_LEN, "%d", valueY);
			lenZ = snprintf(newGyroZ, GYRO_MAX_LEN, "%d", valueZ);
			newDataX.info_data = (uint8_t*)newGyroX;
			newDataX.data_len = lenX;
			newDataY.info_data = (uint8_t*)newGyroY;
			newDataY.data_len = lenY;
			newDataZ.info_data = (uint8_t*)newGyroZ;
			newDataZ.data_len = lenZ;
			UPDATE_GYRO(&cymote_service_handler, &newDataX, &newDataY, &newDataZ, cymote_connection_handle);

			//magnetometer
			valueX = magnetometer_data[0];
			valueY = magnetometer_data[1];
			valueZ = magnetometer_data[2];
			lenX = snprintf(newMagnetX, MAGNET_MAX_LEN, "%d", valueX);
			lenY = snprintf(newMagnetY, MAGNET_MAX_LEN, "%d", valueY);
			lenZ = snprintf(newMagnetZ, MAGNET_MAX_LEN, "%d", valueZ);
			newDataX.info_data = (uint8_t*)newMagnetX;
			newDataX.data_len = lenX;
			newDataY.info_data = (uint8_t*)newMagnetY;
			newDataY.data_len = lenY;
			newDataZ.info_data = (uint8_t*)newMagnetZ;
			newDataZ.data_len = lenZ;
			UPDATE_MAGNET(&cymote_service_handler, &newDataX, &newDataY, &newDataZ, cymote_connection_handle);

			//joystick
			valueX = joystick_data[0];
			valueY = joystick_data[1];
			lenX = snprintf(newJoystickX, JOYSTICK_MAX_LEN, "%d", valueX);
			lenY = snprintf(newJoystickY, JOYSTICK_MAX_LEN, "%d", valueY);
			newDataX.info_data = (uint8_t*)newJoystickX;
			newDataX.data_len = lenX;
			newDataY.info_data = (uint8_t*)newJoystickY;
			newDataY.data_len = lenY;
			UPDATE_JOYSTICK(&cymote_service_handler, &newDataX, &newDataY, cymote_connection_handle);

			//buttons
			valueZ = buttons;
			lenZ = snprintf(newButtons, BUTTONS_MAX_LEN, "%d", valueZ);
			newDataZ.info_data = (uint8_t*)newButtons;
			newDataZ.data_len = lenZ;
			UPDATE_BUTTONS(&cymote_service_handler, &newDataZ, cymote_connection_handle);

			*/
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

		

		/*
		value = (uint8_t)(accelerometer_data[0] & 0x00FF);
		len = snprintf(newValue, ACCEL_X_MAX_LEN, "%d", value);
		printf("Accel X Low Byte: %s\r\n", newValue);
		
		newData.info_data = (uint8_t*)newValue;
		newData.data_len = len;
		UPDATE_ACCEL_X_LOW_DATA(&cymote_service_handler, &newData, cymote_connection_handle);
		*/



		//printf("aX: %d, aY: %d, aZ: %d\r\n", accelerometer_data[0], accelerometer_data[1], accelerometer_data[2]);

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