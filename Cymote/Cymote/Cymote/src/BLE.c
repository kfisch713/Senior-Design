/*
 * BLE.c
 *
 * Created: 11/28/2016 8:22:25 PM
 *  Author: kfisc
 */ 

 #include "BLE.h"

 cymote_characteristic_value_t characteristic_value;
 const uint8_t CYMOTE_SERVICE_UUID[16] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

 const uint8_t BOARD_NAME_UUID[16]     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
 const uint8_t DUMMY_DATA_UUID[16]     = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
 const uint8_t DUMMY_DATA_2_UUID[16]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
 const uint8_t ACCEL_X_UUID[16]		   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04};


 /* Initialize cymote service related information. */
 void cymote_init_service(cymote_service_handler_t* cymote_service){
	 int i;

	 cymote_service->service_handle = 0;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[0].uuid.uuid[i] = CYMOTE_SERVICE_UUID[i];
	 }

	 /* Device name */
	 {
		 cymote_service->service_characteristics[0].char_val_handle = 0;
		 cymote_service->service_characteristics[0].uuid.type = AT_BLE_UUID_128;
		 for(i=0;i<16;i++){
			 cymote_service->service_characteristics[0].uuid.uuid[i] = BOARD_NAME_UUID[i];
		 }
		 cymote_service->service_characteristics[0].properties = AT_BLE_CHAR_READ;
		 memcpy(characteristic_value.board_name, CYMOTE_NAME, BOARD_NAME_LEN);
		 cymote_service->service_characteristics[0].init_value = characteristic_value.board_name;

		 cymote_service->service_characteristics[0].value_init_len = BOARD_NAME_LEN;
		 cymote_service->service_characteristics[0].value_max_len = BOARD_NAME_MAX_LEN;
		 #if BLE_PAIR_ENABLE
		 cymote_service->service_characteristics[0].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
		 #else
		 cymote_service->service_characteristics[0].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
		 #endif
		 cymote_service->service_characteristics[0].user_desc = NULL;           /* user defined name */
		 cymote_service->service_characteristics[0].user_desc_len = 0;
		 cymote_service->service_characteristics[0].user_desc_max_len = 0;
		 cymote_service->service_characteristics[0].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
		 cymote_service->service_characteristics[0].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
		 cymote_service->service_characteristics[0].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
		 cymote_service->service_characteristics[0].user_desc_handle = 0;             /*user desc handles*/
		 cymote_service->service_characteristics[0].client_config_handle = 0;         /*client config handles*/
		 cymote_service->service_characteristics[0].server_config_handle = 0;         /*server config handles*/
		 cymote_service->service_characteristics[0].presentation_format = NULL;
	 }

	 /* Dummy Data */
	 cymote_service->service_characteristics[1].char_val_handle = 0;
	 cymote_service->service_characteristics[1].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[1].uuid.uuid[i] = DUMMY_DATA_UUID[i];
	 }
	 cymote_service->service_characteristics[1].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.dummy, DUMMY_DATA_DEFAULT, DUMMY_DATA_LEN);
	 cymote_service->service_characteristics[1].init_value = characteristic_value.dummy;

	 cymote_service->service_characteristics[1].value_init_len = DUMMY_DATA_LEN;
	 cymote_service->service_characteristics[1].value_max_len = DUMMY_DATA_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[1].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[1].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[1].user_desc = NULL;           /* user defined name */
	 cymote_service->service_characteristics[1].user_desc_len = 0;
	 cymote_service->service_characteristics[1].user_desc_max_len = 0;
	 cymote_service->service_characteristics[1].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[1].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[1].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[1].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[1].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[1].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[1].presentation_format = NULL;
	 
	 /* Dummy Data 2*/
	 cymote_service->service_characteristics[2].char_val_handle = 0;
	 cymote_service->service_characteristics[2].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[2].uuid.uuid[i] = DUMMY_DATA_2_UUID[i];
	 }
	 cymote_service->service_characteristics[2].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.dummy2, DUMMY_DATA_2_DEFAULT, DUMMY_DATA_2_LEN);
	 cymote_service->service_characteristics[2].init_value = characteristic_value.dummy2;

	 cymote_service->service_characteristics[2].value_init_len = DUMMY_DATA_2_LEN;
	 cymote_service->service_characteristics[2].value_max_len = DUMMY_DATA_2_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[2].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[2].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[2].user_desc = NULL;           /* user defined name */
	 cymote_service->service_characteristics[2].user_desc_len = 12;
	 cymote_service->service_characteristics[2].user_desc_max_len = cymote_service->service_characteristics[2].user_desc_len;
	 cymote_service->service_characteristics[2].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[2].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[2].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[2].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[2].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[2].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[2].presentation_format = NULL;


 }

 /* Register cymote_service as primary service instance inside stack. */
 at_ble_status_t cymote_primary_service_define(cymote_service_handler_t *cymote_primary_service)
 {
	 return(at_ble_primary_service_define(&cymote_primary_service->uuid,
	 &cymote_primary_service->service_handle,
	 NULL, 0,
	 cymote_primary_service->service_characteristics, MAX_NUM_CHARACTERISTICS));
 }

 /* Update the cymote characteristic value after defining the services using cymote_primary_service_define */
 at_ble_status_t cymote_info_update(cymote_service_handler_t *cymote_serv , cymote_info_type info_type, cymote_info_data* info_data, at_ble_handle_t conn_handle)
 {
	 //printf("flag_cymote_info_update: 1\r\n");
	 if (info_data->data_len > cymote_serv->service_characteristics[info_type].value_max_len)
	 {
		 DBG_LOG("invalid length parameter");
		 return AT_BLE_FAILURE;
	 }
	 //printf("flag_cymote_info_update: 2\r\n");
	 //updating application data
	 //printf("info_type: %d\r\n", info_type);
	 //printf("info_data.info_data: %d\r\n", info_data->info_data);
	 //printf("info_data.data_len: %d\r\n", info_data->data_len);
	 //printf("old value = %d\r\n", *(cymote_serv->service_characteristics[info_type].init_value));
	 memcpy(cymote_serv->service_characteristics[info_type].init_value, info_data->info_data, info_data->data_len);
	 //printf("flag_cymote_info_update: 3\r\n");
	 //printf("new value = %d\r\n", *(cymote_serv->service_characteristics[info_type].init_value));
	 //updating the characteristic value
	 if ((at_ble_characteristic_value_set(cymote_serv->service_characteristics[info_type].char_val_handle, info_data->info_data, info_data->data_len)) != AT_BLE_SUCCESS){
		 //printf("flag_cymote_info_update: 4.1\r\n");
		 DBG_LOG("updating the characteristic failed\r\n");
		 } else {
		 //printf("flag_cymote_info_update: 4.2\r\n");
		 return AT_BLE_SUCCESS;
	 }
	 //printf("flag_cymote_info_update: 4\r\n");
	 ALL_UNUSED(conn_handle); //not sure what this does. -Kyle
	 //printf("flag_cymote_info_update: 5\r\n");
	 return AT_BLE_FAILURE;
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

 