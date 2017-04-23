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

/*
	Top to Bottom
	CS_AG	White	EXT1 15
	MOSI	Blue	EXT1 16
	MISO	Green	EXT1 17
	SCLK	Purple	EXT1 18
	GND		Gray	EXT1 19
	VCC		Orange	EXT1 20
*/


#define DONT_USE_CMSIS_INIT
#include <asf.h>
#include "BLE.h"
#include "LSM9DS1.h"
#include "timer_hw.h"
#include "button.h"
#include "console_serial.h"
#include "time.h"
#include <stdint.h>
#include "LED.h"
#include "cymote_selector.h"
#include "ADC_GPIO.h"


#define DATA_BUFFER_LENGTH 20
#define LENGTH_OF_DATA_IN_STRING_FORM 5

uint8_t prepare_send_buffer(uint8_t buffer[DATA_BUFFER_LENGTH], uint16_t data1, uint16_t data2, uint16_t data3);
uint8_t prepare_send_buffer_timer(uint8_t buffer[DATA_BUFFER_LENGTH], uint64_t data);
void default_gpio_pin_set(void);


/* Globals */
cymote_service_handler_t cymote_service_handler;
at_ble_handle_t cymote_connection_handle;
struct uart_module console_instance;
uint16_t accelerometer_data[3];
uint16_t gyroscope_data[3];
uint16_t magnetometer_data[3];
uint16_t joystick_data[2];
uint16_t buttons;
volatile uint64_t time_ms = 0;
cymote_service_handler_t cymote_gatt_service_handle;

/* Blank BLE message timer callback function */
static void ble_timer_callback_fn(void)
{
	/* Add timer callback functionality here */

	//This breaks out of waiting for a BLE event
	send_plf_int_msg_ind(USER_TIMER_CALLBACK, TIMER_EXPIRED_CALLBACK_TYPE_DETECT, NULL, 0);
	//DBG_LOG("BLE timer callback");
}

/* Counts time to send via BLE message */
static void time_timer_callback_fn(void){
	time_ms += TIMER_UPDATE_IN_MS;
	//DBG_LOG("time timer callback");
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
	at_ble_service_t cymote_service;
	cymote_characteristic_handle_t cymote_handles;
	cymote_service_data_t cymote_data; //this is just creating the place in memory that everything will be stored in.
	uint16_t valueX, valueY, valueZ;
	uint8_t len;
	uint8_t temp[DATA_BUFFER_LENGTH];
	bool red, blue, green;
	bool butt1_pin_state = false;
	bool butt2_pin_state = false;
	bool butt3_pin_state = false;
	bool butt4_pin_state = false;
	uint16_t adc4_result;
	uint16_t adc3_result;

	//if using Cymote Beta then set the gpio muxes
	if(!ALPHA_CONNECTIONS){
		default_gpio_pin_set();
	}
	

	/* LED initialization */
	setup_LED();

    /* Initialize the SAM system */
	platform_driver_init();
	acquire_sleep_lock();
	system_clock_config(CLOCK_RESOURCE_XO_26_MHZ, CLOCK_FREQ_26_MHZ);

	/* Initialize serial console. */
	//BLE uses DBG_LOG a lot which requires serial_console_init() to be called before it.
	serial_console_init();
	
	
	/* Hardware timer. */
	hw_timer_init();
	hw_timer_register_callback(ble_timer_callback_fn, time_timer_callback_fn);
	hw_timer_start(BLE_UPDATE_INTERVAL, TIMER_UPDATE_IN_MS);
	
	/* Button initialization. */
	gpio_init();
	button_init();
	button_register_callback(button_cb);

	/* LSM9DS0 S configuration. */
	if(USE_9OF){
		red = true;
		green = true;
		blue = false;
		configure_gpio();
		
		/* Start up SPI and read the status register. */
		uint8_t receive = 0x00;
		ag_read_byte(WHO_AM_I_XM);
		receive = ag_read_byte(WHO_AM_I_XM);
		printf("Should be 0x68: 0x%x\r\n", receive);

		/* Initialize accelerometer control registers. */
		init_accelerometer();
	
		/* Initialize magnetometer control registers. */
		init_magnetometer();
	
		/* Initialize gyroscope control registers. */
		init_gyroscope();
	}
	
	if(USE_BLE){
		DBG_LOG("Initializing BLE Application");
	
		/* Initialize the BLE chip and set the device address. */
		at_ble_addr_t addr;
		addr.type = AT_BLE_ADDRESS_PUBLIC;
		int i;
		for(i = 0; i < AT_BLE_ADDR_LEN; i++){
			addr.addr[i] = 0;
		}
		addr.addr[0] = 2;
		ble_device_init(&addr);
		DBG_LOG("made it past ble device init");


		if((status = cymote_service_init(&cymote_service, &cymote_data, &cymote_handles)) != AT_BLE_SUCCESS){
			DBG_LOG("Service definition failed, reason %x", status);
		}
		else {
			DBG_LOG("Service definition success");
		}

		device_information_advertise();

		/* Register callbacks for gap related events. */
		ble_mgr_events_callback_handler(REGISTER_CALL_BACK, BLE_GAP_EVENT_TYPE, cymote_app_gap_cb);
	}

	/* Initialize Joystick registers. */ 
	configure_adc_4();
	configure_adc_3();
	configure_gpio_pins();

	//if it changes to green, then the board has made it into the while loop
	red = false;
	green = true;
	blue = false;
	set_LED(red, green, blue); //green

    /* Main working loop. */
    while (1) 
    {
		if(USE_BLE){
			/* BLE Event task */
			ble_event_task(BLE_EVENT_TIMEOUT);
		
			/* Update BLE data */
			if(USE_9OF){
				get_raw_accelerometer(accelerometer_data);
				get_raw_gyroscope(gyroscope_data);
				get_raw_magnetometer(magnetometer_data);
				get_button_data(&butt1_pin_state, &butt2_pin_state, &butt3_pin_state, &butt4_pin_state);
				joystick_data[0] = adc3_result;
				joystick_data[1] = adc4_result;
				buttons = 0; // reset
				buttons = butt4_pin_state << 3 | butt3_pin_state << 2 | butt2_pin_state << 1 | butt1_pin_state;
				//DBG_LOG("buttons: %d, 1: %d, 2: %d, 3: %d, 4: %d", buttons, butt1_pin_state, butt2_pin_state, butt3_pin_state, butt4_pin_state);
				
			}
			else{
				//dummy data
				accelerometer_data[0] = 10000;
				accelerometer_data[1] = 10001;
				accelerometer_data[2] = 10002;
				gyroscope_data[0] = 123;
				gyroscope_data[1] = 456;
				gyroscope_data[2] = 789;
				magnetometer_data[0] = 30000;
				magnetometer_data[1] = 30001;
				magnetometer_data[2] = 30002;
				joystick_data[0] = 42;
				joystick_data[1] = 24;
				buttons = 4;
				time_ms = 111;
			}
			
			//reset temp
			memset(temp, NULL, DATA_BUFFER_LENGTH);

			valueX = accelerometer_data[0];
			valueY = accelerometer_data[1];
			valueZ = accelerometer_data[2];
			len = prepare_send_buffer(temp, valueX, valueY, valueZ);
			status = at_ble_characteristic_value_set(cymote_handles.accel_handle, temp, len);
			//DBG_LOG("status 1: %x", status);
			//DBG_LOG("%s", temp);			
			
			//gyroscope
			valueX = gyroscope_data[0];
			valueY = gyroscope_data[1];
			valueZ = gyroscope_data[2];
			len = prepare_send_buffer(temp, valueX, valueY, valueZ);
			status = at_ble_characteristic_value_set(cymote_handles.gyro_handle, temp, len);
			//DBG_LOG("status 2: %x", status);
			//DBG_LOG("%s", temp);

			//magnetometer
			valueX = magnetometer_data[0];
			valueY = magnetometer_data[1];
			valueZ = magnetometer_data[2];
			len = prepare_send_buffer(temp, valueX, valueY, valueZ);
			status = at_ble_characteristic_value_set(cymote_handles.magnet_handle, temp, len);
			//DBG_LOG("status 3: %x", status);
			//DBG_LOG("%s", temp);
			
			//joystick and buttons
			valueX = joystick_data[0];
			valueY = joystick_data[1];
			valueZ = buttons;
			len = prepare_send_buffer(temp, valueX, valueY, valueZ);
			status = at_ble_characteristic_value_set(cymote_handles.joystick_buttons_handle, temp, len);
			//DBG_LOG("status 4: %x", status);
			//DBG_LOG("%s", temp);
			
			//time
			//time updated from an interrupt
			len = prepare_send_buffer_timer(temp, time_ms);
			status = at_ble_characteristic_value_set(cymote_handles.time_handle, temp, len);
			//DBG_LOG("status 5: %x", status);
			//DBG_LOG("%s", temp);
		}
		 
		

		if (butt1_pin_state){
			//reset the adc
			configure_adc_4();
			configure_adc_3();

			// Read Horizontal
			do {
			} while (adc_read(ADC_INPUT_CH_GPIO_MS1, &adc3_result) == STATUS_BUSY);

			// Read Vertical
			do{
			}while (adc_read(ADC_INPUT_CH_GPIO_MS2, &adc4_result) == STATUS_BUSY);

			int delay = adc3_result * 300;
			while (delay > 0){
				delay--;
			}
			
			red = true;
			set_LED(red, green, blue);

			delay = adc3_result * 300;
			while (delay > 0){
				delay--;
			}
			
			red = false;
			set_LED(red, green, blue);

			if (adc4_result > 2000){
				green = true;
				set_LED(red, green, blue);
			}
			else{
				green = false;
				set_LED(red, green, blue);
			}
			if (adc4_result < 500){
				blue = true;
				set_LED(red, green, blue);
			}
			else{
				blue = false;
				set_LED(red, green, blue);
			}
		}
		else{
			if (butt2_pin_state){
				red = true;
				set_LED(red, green, blue);
				}else{
				red = false;
				set_LED(red, green, blue);
			}
			
			if (butt3_pin_state){
				green = true;
				set_LED(red, green, blue);
				}else{
				green = false;
				set_LED(red, green, blue);
			}
			
			if (butt4_pin_state){
				blue = true;
				set_LED(red, green, blue);
				}else{
				blue = false;
				set_LED(red, green, blue);
			}
		}
    }
}

/* Helper function to put data into a buffer to be sent over BLE. Space delimited.
   Returns the length of good data in buffer.
*/
uint8_t prepare_send_buffer(uint8_t buffer[DATA_BUFFER_LENGTH], uint16_t data1, uint16_t data2, uint16_t data3){
	char temp1[DATA_BUFFER_LENGTH], temp2[DATA_BUFFER_LENGTH], temp3[DATA_BUFFER_LENGTH];
	memset(buffer, NULL, DATA_BUFFER_LENGTH);
	
	uint8_t len1 = snprintf(temp1, DATA_BUFFER_LENGTH, "%d", data1);
	uint8_t len2 = snprintf(temp2, DATA_BUFFER_LENGTH, "%d", data2);
	uint8_t len3 = snprintf(temp3, DATA_BUFFER_LENGTH, "%d", data3);

	memcpy(buffer, temp1, len1);
	memcpy(buffer+len1, " ", 1);
	memcpy(buffer+len1+1, temp2, len2);
	memcpy(buffer+len1+len2+1, " ", 1);
	memcpy(buffer+len1+len2+2, temp3, len3);

	return len1+len2+len3+2;
}

/* Helper function to put data into a buffer to be sent over BLE. Space delimited.
   This function is specifically for time b/c it takes a uint32_t.
   Returns the length of good data in buffer.
*/
uint8_t prepare_send_buffer_timer(uint8_t buffer[DATA_BUFFER_LENGTH], uint64_t data){
	memset(buffer, NULL, DATA_BUFFER_LENGTH);

	char temp[DATA_BUFFER_LENGTH];
	uint8_t len = snprintf(temp, DATA_BUFFER_LENGTH, "%04x%04x%04x%04x", (uint16_t)(data>>48), (uint16_t)(data>>32), (uint16_t)(data>>16), (uint16_t)data);
	
	memcpy(buffer, temp, len);
	return len;
}

void default_gpio_pin_set(){
	struct gpio_config config;
	gpio_get_config_defaults(&config);

	//see the pinout datasheet
	gpio_pinmux_cofiguration(PIN_LP_GPIO_0,  GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_1,  GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_2,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_3,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_4,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_5,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_6,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_7,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_8,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_9,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_10, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_11, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_12, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_13, GPIO_PINMUX_SEL_2);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_14, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_15, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_16, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_17, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_18, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_19, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_20, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_22, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_LP_GPIO_23, GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_AO_GPIO_0,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_AO_GPIO_1,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_AO_GPIO_2,  GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_GPIO_MS1,   GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_GPIO_MS2,   GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_GPIO_MS3,   GPIO_PINMUX_SEL_0);
	gpio_pinmux_cofiguration(PIN_GPIO_MS4,   GPIO_PINMUX_SEL_0);



	//setup pins as input by default. they can be changed later.
	config.direction = GPIO_PIN_DIR_INPUT;
	config.input_pull = GPIO_PIN_PULL_NONE;
	gpio_pin_set_config(PIN_LP_GPIO_0,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_1,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_2,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_3,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_4,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_5,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_6,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_7,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_8,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_9,  &config);
	gpio_pin_set_config(PIN_LP_GPIO_10, &config);
	gpio_pin_set_config(PIN_LP_GPIO_11, &config);
	gpio_pin_set_config(PIN_LP_GPIO_12, &config);
	gpio_pin_set_config(PIN_LP_GPIO_13, &config);
	gpio_pin_set_config(PIN_LP_GPIO_14, &config);
	gpio_pin_set_config(PIN_LP_GPIO_15, &config);
	gpio_pin_set_config(PIN_LP_GPIO_16, &config);
	gpio_pin_set_config(PIN_LP_GPIO_17, &config);
	gpio_pin_set_config(PIN_LP_GPIO_18, &config);
	gpio_pin_set_config(PIN_LP_GPIO_19, &config);
	gpio_pin_set_config(PIN_LP_GPIO_20, &config);
	gpio_pin_set_config(PIN_LP_GPIO_22, &config);
	gpio_pin_set_config(PIN_LP_GPIO_23, &config);
	gpio_pin_set_config(PIN_AO_GPIO_0,  &config);
	gpio_pin_set_config(PIN_AO_GPIO_1,  &config);
	gpio_pin_set_config(PIN_AO_GPIO_2,  &config);
	gpio_pin_set_config(PIN_GPIO_MS1,   &config);
	gpio_pin_set_config(PIN_GPIO_MS2,   &config);
	gpio_pin_set_config(PIN_GPIO_MS3,   &config);
	gpio_pin_set_config(PIN_GPIO_MS4,   &config);

	
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