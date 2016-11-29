/**
 * \file
 *
 * \brief BLE Startup Template
 *
 * Copyright (c) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
 * Support</a>
 */

/**
 * \mainpage
 * \section preface Preface
 * This is the reference manual for the Startup Template
 */
/*- Includes ---------------------------------------------------------------*/
#include <asf.h>
#include "platform.h"
#include "at_ble_api.h"
#include "console_serial.h"
#include "timer_hw.h"
#include "ble_manager.h"
#include "ble_utils.h"
#include "button.h"
#include "startup_template_app.h"
#include "cymote.h"

/* Globals */
cymote_service_handler_t cymote_service_handler;
at_ble_handle_t cymote_connection_handle;
uint8_t dummy_data_counter = 0;
uint8_t dummy_data_counter_old = 0;
uint8_t dummy_data_2_state = 0;
uint8_t dummy_data[] = {'A', 'B', 'C', 'D', 'C', 'E', 'a'};


/* timer callback function */
static void timer_callback_fn(void)
{
	/* Add timer callback functionality here */
	//Timer was never started
}

static void button_cb(void)
{
	/* Add button callback functionality here */
	
	if (dummy_data_counter >= (unsigned int)(sizeof(dummy_data)/sizeof(dummy_data[0])-1) ){
		dummy_data_counter = 0;
	}
	else dummy_data_counter++;
	
	dummy_data_2_state = 1;
	
	//This breaks out of waiting for a BLE event
	send_plf_int_msg_ind(USER_TIMER_CALLBACK, TIMER_EXPIRED_CALLBACK_TYPE_DETECT, NULL, 0);
	
	printf("Button CB\r\n");

}

/* Advertisement data set and Advertisement start */
static at_ble_status_t device_information_advertise(void)
{
	at_ble_status_t status = AT_BLE_FAILURE;
	
	if((status = ble_advertisement_data_set()) != AT_BLE_SUCCESS)
	{
		DBG_LOG("advertisement data set failed reason :%d",status);
		return status;
	}
	
	/* Start of advertisement */
	if((status = at_ble_adv_start(AT_BLE_ADV_TYPE_UNDIRECTED, AT_BLE_ADV_GEN_DISCOVERABLE, NULL, AT_BLE_ADV_FP_ANY, APP_FAST_ADV, APP_ADV_TIMEOUT, 0)) == AT_BLE_SUCCESS)
	{
		DBG_LOG("BLE Started Adv");
		return AT_BLE_SUCCESS;
	}
	else
	{
		DBG_LOG("BLE Adv start Failed status :%d",status);
	}
	return status;
}

/* Callback registered for AT_BLE_PAIR_DONE event from stack */
static at_ble_status_t ble_paired_app_event(void *param)
{
	at_ble_pair_done_t *at_ble_pair_done = (at_ble_pair_done_t *)param;
	LED_On(LED0);
	//hw_timer_start(FIRMWARE_UPDATE_INTERVAL);
	cymote_connection_handle = at_ble_pair_done->handle;
	return AT_BLE_SUCCESS;
}

/* Callback registered for AT_BLE_DISCONNECTED event from stack */
static at_ble_status_t ble_disconnected_app_event(void *param)
{
	//hw_timer_stop();
	//timer_cb_done = false;
	LED_Off(LED0);
	device_information_advertise();
	ALL_UNUSED(param);
	return AT_BLE_SUCCESS;
}

/* Callback registered for AT_BLE_CONNECTED event from stack */
static at_ble_status_t ble_connected_app_event(void *param)
{
	#if !BLE_PAIR_ENABLE
	ble_paired_app_event(param);
	#else
	ALL_UNUSED(param);
	#endif
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

	platform_driver_init();
	acquire_sleep_lock();

	/* Initialize serial console */
	serial_console_init();
	
	/* Hardware timer */
	hw_timer_init();
	
	/* button initialization */
	gpio_init();
	button_init();
	button_register_callback(button_cb);
	
	hw_timer_register_callback(timer_callback_fn);

	DBG_LOG("Initializing BLE Application");
	
	/* initialize the BLE chip  and Set the Device Address */
	at_ble_addr_t addr;
	addr.type = AT_BLE_ADDRESS_PUBLIC;
	int i;
	for(i=0; i<AT_BLE_ADDR_LEN; i++){
		addr.addr[i] = 0;
	}
	addr.addr[0]=1;
	ble_device_init(&addr);

	cymote_init_service(&cymote_service_handler);
	if ((status = cymote_primary_service_define(&cymote_service_handler)) != AT_BLE_SUCCESS)
	{
		DBG_LOG("Device Information Service definition failed,reason %x",status);
	}

	device_information_advertise();

	/* Register callbacks for gap related events */
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK,
									BLE_GAP_EVENT_TYPE,
									cymote_app_gap_cb);
	
	/* Set ULP mode */
	//ble_set_ulp_mode(BLE_ULP_MODE_SET);

	cymote_info_data newData;

	while(1)
	{
		
		/* BLE Event task */
		ble_event_task(BLE_EVENT_TIMEOUT);
		
		/* Write application task */
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
			int value = rand()%1000;
			uint8_t len = snprintf(newValue, DUMMY_DATA_2_MAX_LEN, "%d", value);
			printf("newValue: %s\r\n", newValue);
						
			newData.info_data = (uint8_t*)newValue;
			newData.data_len = len;
			UPDATE_DUMMY_DATA_2(&cymote_service_handler, &newData, cymote_connection_handle);
		}
		

	}

}

