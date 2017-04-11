/*
 * BLE.c
 *
 * Created: 11/28/2016 8:22:25 PM
 *  Author: kfisc
 */ 

 #include "BLE.h"

 const uint8_t CYMOTE_SERVICE_UUID[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x33, 0x37, 0x31, 0x79, 0x61, 0x6d};


 const uint8_t ACCEL_UUID[16]				= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01};
 const uint8_t GYRO_UUID[16]				= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x02};
 const uint8_t MAGNET_UUID[16]				= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03};
 const uint8_t JOYSTICK_BUTTONS_UUID[16]	= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x04};
 const uint8_t TIME_UUID[16]				= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x05};

 
 
 /* Initialize a service and return handles to its characteristics so they can be updated. */
 at_ble_status_t cymote_service_init(at_ble_service_t *cymote_service, cymote_service_data_t* data, cymote_characteristic_handle_t *cymote_handles){
	int i;
	at_ble_status_t status = AT_BLE_FAILURE;
	
	//setup the characteristic uuids
	characteristic_uuid_init(data->characteristic_uuids);
	DBG_LOG("made it past characteristic uuid init");

	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		data->characteristic_values[i].init_value = 0;
		data->characteristic_values[i].len = 20;
		data->characteristic_values[i].permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		data->characteristic_values[i].properties = AT_BLE_CHAR_READ;
		data->characteristic_values[i].uuid = data->characteristic_uuids[i];
	}
	DBG_LOG("made it past characteristic value loop");
	
	//initializes the characteristic descriptions
	characteristic_description_init(data->characteristic_description);
	DBG_LOG("made it past characteristic description init");
	fflush(stdout);
	
	//characteristic presentation is null so it is being thrown into this loop. Consider bringing it out of the loop
	//if it is going to be set to something.
	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		data->characteristic_presentation[i].description = AT_BLE_PRES_FORMAT_UINT16; 
		data->characteristic_presentation[i].exponent = 0; 
		data->characteristic_presentation[i].unit = 4; // unit is uint8_t 
		data->characteristic_presentation[i].name_space = 0; // not a SIG. Not a Bluetooth SIG assigned number 
		data->characteristic_presentation[i].description = 0; // not a SIG. Not a Bluetooth SIG assigned number
	}

	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		
		data->client_config[i].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		data->server_config[i].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	}
	DBG_LOG("made it past server and client config");

	//initialize attribute descriptions
	characteristic_attribute_descriptions_init(data->attribute_descriptions);
	DBG_LOG("made it past characteristic attribute descriptions init");
	
	//assign characteristics their properties
	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		data->characteristics[i].char_val = data->characteristic_values[i];
		data->characteristics[i].user_desc = data->characteristic_description[i];
		data->characteristics[i].presentation_format = &(data->characteristic_presentation[i]);
		data->characteristics[i].client_config_desc = data->client_config[i];
		data->characteristics[i].server_config_desc = data->server_config[i];
		data->characteristics[i].additional_desc_list = data->attribute_descriptions[i];
		data->characteristics[i].additional_desc_count = NUMBER_ATTRIBUTE_DESCRIPTIONS;
	}
	DBG_LOG("made it past assigning characteristics their properties");


	//setup the service uuid
	data->service_uuid.type=AT_BLE_UUID_128;
	for(i=0;i<16;i++){
		data->service_uuid.uuid[i] = CYMOTE_SERVICE_UUID[i];
	}
	DBG_LOG("made it past service uuid init");

	/* wrap everything into the service */
	cymote_service->type	       = PRIMARY_SERVICE;
	cymote_service->uuid           = data->service_uuid;
	cymote_service->perm	       = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	cymote_service->inc_list       = NULL;    //no secondary services included
	cymote_service->included_count = 0;       //no secondary services included
	cymote_service->char_list      = data->characteristics;
	cymote_service->char_count     = MAX_NUM_CHARACTERISTICS;
	DBG_LOG("made it past wrapping service");

	status = at_ble_service_define(cymote_service);
	DBG_LOG("made it past service define");

	//assign the address of the handles so characteristics can be updated
	cymote_handles->accel_handle				= cymote_service->char_list[0].char_val.handle;
	cymote_handles->gyro_handle					= cymote_service->char_list[1].char_val.handle;
	cymote_handles->magnet_handle				= cymote_service->char_list[2].char_val.handle;
	cymote_handles->joystick_buttons_handle		= cymote_service->char_list[3].char_val.handle;
	cymote_handles->time_handle					= cymote_service->char_list[4].char_val.handle;
	DBG_LOG("made it past saving characteristic handles");


	return status;
 }

 /* Add the service to the profile */ 
 at_ble_status_t cymote_service_define(at_ble_service_t *service){
	at_ble_status_t status_of_service_define = at_ble_service_define(service);
	return status_of_service_define;
 }

 /* Initializes all of the characteristic UUIDs */
 void characteristic_uuid_init(at_ble_uuid_t characteristic_uuids[MAX_NUM_CHARACTERISTICS]){
	int i;
	
	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		//DBG_LOG("Loop1: %d", i);
		characteristic_uuids[i].type = AT_BLE_UUID_128;
	}


	for(i=0;i<16;i++){
		//DBG_LOG("Loop2: %d", i);
		characteristic_uuids[0].uuid[i] = ACCEL_UUID[i];
		characteristic_uuids[1].uuid[i] = GYRO_UUID[i];
		characteristic_uuids[2].uuid[i] = MAGNET_UUID[i];
		characteristic_uuids[3].uuid[i] = JOYSTICK_BUTTONS_UUID[i];
		characteristic_uuids[4].uuid[i] = TIME_UUID[i];
	}
 }

 /* Initializes the characteristic description list */
 void characteristic_description_init(at_ble_user_desc_t characteristic_descriptions[MAX_NUM_CHARACTERISTICS]){
	int i;

	const char* desc[MAX_NUM_CHARACTERISTICS] = {"Acceleration XYZ",
											     "Gyroscope XYZ",
											     "Magnetometer XYZ",
											     "Joystick and buttons JJB",
											     "Time"};

	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		DBG_LOG("Iteration: %d", i);
		characteristic_descriptions[i].user_description = (uint8_t*)desc[i];
		characteristic_descriptions[i].permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		characteristic_descriptions[i].len = strlen(desc[i]);
		DBG_LOG("%s, %d", desc[i], strlen(desc[i]));
	}
	DBG_LOG("out of loop");

	return;

 }

/* Initializes the characteristic attribute descriptions. */
void characteristic_attribute_descriptions_init(at_ble_generic_att_desc_t descriptions[MAX_NUM_CHARACTERISTICS][NUMBER_ATTRIBUTE_DESCRIPTIONS]){
	int i, j;
	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		for(j=0;j<NUMBER_ATTRIBUTE_DESCRIPTIONS;j++){
			//arbitrary number. I just made it up. I don't know what it does.
			descriptions[i][j].desc_val_length = 20;
			descriptions[i][j].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		}
	}

	//Use the data UUID but adding in a 1
	for(i=0;i<NUMBER_ATTRIBUTE_DESCRIPTIONS;i++){
		for(j=0;j<16;j++){
			descriptions[0][i].uuid.uuid[j] = ACCEL_UUID[i];
			descriptions[1][i].uuid.uuid[j] = GYRO_UUID[i];
			descriptions[2][i].uuid.uuid[j] = MAGNET_UUID[i];
			descriptions[3][i].uuid.uuid[j] = JOYSTICK_BUTTONS_UUID[i];
			descriptions[4][i].uuid.uuid[j] = TIME_UUID[i];
			
		}
		descriptions[0][i].uuid.type = AT_BLE_UUID_128;
		descriptions[1][i].uuid.type = AT_BLE_UUID_128;
		descriptions[2][i].uuid.type = AT_BLE_UUID_128;
		descriptions[3][i].uuid.type = AT_BLE_UUID_128;
		descriptions[4][i].uuid.type = AT_BLE_UUID_128;
	}
	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		for(j=0;j<NUMBER_ATTRIBUTE_DESCRIPTIONS;j++){
		descriptions[i][j].uuid.uuid[13] = 0x10;
		}
	}
	
}


 /* Advertisement data set and Advertisement start */
 at_ble_status_t device_information_advertise(void)
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

 ///* Callback registered for AT_BLE_PAIR_DONE event from stack */
 //static at_ble_status_t ble_paired_app_event(void *param)
 //{
	 //at_ble_pair_done_t *at_ble_pair_done = (at_ble_pair_done_t *)param;
	 //LED_On(LED0);
	 ////hw_timer_start(BLE_UPDATE_INTERVAL);
	 //cymote_connection_handle = at_ble_pair_done->handle;
	 //return AT_BLE_SUCCESS;
 //}

 /* Callback registered for AT_BLE_DISCONNECTED event from stack */
 at_ble_status_t ble_disconnected_app_event(void *param)
 {
	 //hw_timer_stop();
	 //timer_cb_done = false;
	 LED_Off(LED0);
	 device_information_advertise();
	 ALL_UNUSED(param);
	 return AT_BLE_SUCCESS;
 }

 /* Callback registered for AT_BLE_CONNECTED event from stack */
 at_ble_status_t ble_connected_app_event(void *param)
 {
	 #if !BLE_PAIR_ENABLE
	 ble_paired_app_event(param);
	 #else
	 ALL_UNUSED(param);
	 #endif
	 return AT_BLE_SUCCESS;
 }

 