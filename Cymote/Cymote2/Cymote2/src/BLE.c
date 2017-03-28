/*
 * BLE.c
 *
 * Created: 11/28/2016 8:22:25 PM
 *  Author: kfisc
 */ 

 #include "BLE.h"

cymote_characteristic_value_t characteristic_value;
 const uint8_t CYMOTE_SERVICE_UUID[16] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
 //{0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

 const uint8_t ACCEL_X_UUID[16]		   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01};
 const uint8_t ACCEL_Y_UUID[16]	       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x01};
 const uint8_t ACCEL_Z_UUID[16]	       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01};
 const uint8_t GYRO_X_UUID[16]	       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x02};
 const uint8_t GYRO_Y_UUID[16]	       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02};
 const uint8_t GYRO_Z_UUID[16]	       = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x02};
 const uint8_t MAGNET_X_UUID[16]	   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03};
 const uint8_t MAGNET_Y_UUID[16]	   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x03};
 const uint8_t MAGNET_Z_UUID[16]	   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x03};
 const uint8_t JOYSTICK_X_UUID[16]	   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x04};
 const uint8_t JOYSTICK_Y_UUID[16]	   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x04};
 const uint8_t BUTTONS_UUID[16]        = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x05};


 /* Initialize cymote service related information. */
 void cymote_init_service(cymote_service_handler_t* cymote_service){
	 int i;
	 int index = 0;
	 
	 cymote_service->service_handle = 0;
	 cymote_service->uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		cymote_service->uuid.uuid[i] = 1;//CYMOTE_SERVICE_UUID[i];
		printf("%u\r\n", cymote_service->uuid.uuid[i]);
	 }
	 
	 /*
	 cymote_service->uuid.type = AT_BLE_UUID_16;
	 for(i=0;i<16;i=i+4){
		 cymote_service->uuid.uuid[i] = CYMOTE_SERVICE_UUID[i];
	 }*/

	  
	 /* Accelerometer x axis raw data */
	 index = 0;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		cymote_service->service_characteristics[index].uuid.uuid[i] = ACCEL_X_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.accel_x, ACCEL_DEFAULT, ACCEL_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.accel_x;
	 
	 cymote_service->service_characteristics[index].value_init_len = ACCEL_LEN;
	 cymote_service->service_characteristics[index].value_max_len = ACCEL_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;           /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = 0;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
 	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
 	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
 	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;


	 /* Accelerometer y axis raw data */
	 index = 1;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = ACCEL_Y_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.accel_y, ACCEL_DEFAULT, ACCEL_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.accel_y;

	 cymote_service->service_characteristics[index].value_init_len = ACCEL_LEN;
	 cymote_service->service_characteristics[index].value_max_len = ACCEL_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;           /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = 0;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;
	 
	 /* Accelerometer z axis raw data */
	 index = 2;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = ACCEL_Z_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.accel_z, ACCEL_DEFAULT, ACCEL_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.accel_z;

	 cymote_service->service_characteristics[index].value_init_len = ACCEL_LEN;
	 cymote_service->service_characteristics[index].value_max_len = ACCEL_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;           /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Gyroscope x axis raw data */
	 index = 3;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = GYRO_X_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.gyro_x, GYRO_DEFAULT, GYRO_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.gyro_x;

	 cymote_service->service_characteristics[index].value_init_len = GYRO_LEN;
	 cymote_service->service_characteristics[index].value_max_len = GYRO_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Gyroscope y axis raw data */
	 index = 4;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = GYRO_Y_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.gyro_y, GYRO_DEFAULT, GYRO_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.gyro_y;

	 cymote_service->service_characteristics[index].value_init_len = GYRO_LEN;
	 cymote_service->service_characteristics[index].value_max_len = GYRO_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Gyroscope z axis raw data */
	 index = 5;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = GYRO_Z_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.gyro_z, GYRO_DEFAULT, GYRO_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.gyro_z;

	 cymote_service->service_characteristics[index].value_init_len = GYRO_LEN;
	 cymote_service->service_characteristics[index].value_max_len = GYRO_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Magnetometer x axis raw data */
	 index = 6;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = MAGNET_X_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.magnet_x, MAGNET_DEFAULT, MAGNET_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.magnet_x;

	 cymote_service->service_characteristics[index].value_init_len = MAGNET_LEN;
	 cymote_service->service_characteristics[index].value_max_len = MAGNET_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Magnetometer y axis raw data */
	 index = 7;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = MAGNET_Y_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.magnet_y, MAGNET_DEFAULT, MAGNET_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.magnet_y;

	 cymote_service->service_characteristics[index].value_init_len = MAGNET_LEN;
	 cymote_service->service_characteristics[index].value_max_len = MAGNET_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Magnetometer z axis raw data */
	 index = 8;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = MAGNET_Z_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.magnet_z, MAGNET_DEFAULT, MAGNET_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.magnet_z;

	 cymote_service->service_characteristics[index].value_init_len = MAGNET_LEN;
	 cymote_service->service_characteristics[index].value_max_len = MAGNET_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Joystick x axis raw data */
	 index = 9;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = JOYSTICK_X_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.joystick_x, JOYSTICK_DEFAULT, JOYSTICK_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.joystick_x;

	 cymote_service->service_characteristics[index].value_init_len = JOYSTICK_LEN;
	 cymote_service->service_characteristics[index].value_max_len = JOYSTICK_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Joystick y axis raw data */
	 index = 10;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = JOYSTICK_Y_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.joystick_y, JOYSTICK_DEFAULT, JOYSTICK_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.joystick_y;

	 cymote_service->service_characteristics[index].value_init_len = JOYSTICK_LEN;
	 cymote_service->service_characteristics[index].value_max_len = JOYSTICK_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;

	 /* Buttons raw data */
	 index = 11;
	 cymote_service->service_characteristics[index].char_val_handle = index;
	 cymote_service->service_characteristics[index].uuid.type = AT_BLE_UUID_128;
	 for(i=0;i<16;i++){
		 cymote_service->service_characteristics[index].uuid.uuid[i] = BUTTONS_UUID[i];
	 }
	 cymote_service->service_characteristics[index].properties = AT_BLE_CHAR_READ;
	 memcpy(characteristic_value.buttons, BUTTONS_DEFAULT, BUTTONS_LEN);
	 cymote_service->service_characteristics[index].init_value = characteristic_value.buttons;

	 cymote_service->service_characteristics[index].value_init_len = BUTTONS_LEN;
	 cymote_service->service_characteristics[index].value_max_len = BUTTONS_MAX_LEN;
	 #if BLE_PAIR_ENABLE
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_REQ_AUTHN_NO_AUTHR;   /* permissions */
	 #else
	 cymote_service->service_characteristics[index].value_permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;   /* permissions */
	 #endif
	 cymote_service->service_characteristics[index].user_desc = NULL;            /* user defined name */
	 cymote_service->service_characteristics[index].user_desc_len = 0;
	 cymote_service->service_characteristics[index].user_desc_max_len = cymote_service->service_characteristics[index].user_desc_len;
	 cymote_service->service_characteristics[index].user_desc_permissions = AT_BLE_ATTR_NO_PERMISSIONS;             /*user description permissions*/
	 cymote_service->service_characteristics[index].client_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*client config permissions*/
	 cymote_service->service_characteristics[index].server_config_permissions = AT_BLE_ATTR_NO_PERMISSIONS;         /*server config permissions*/
	 cymote_service->service_characteristics[index].user_desc_handle = 0;             /*user desc handles*/
	 cymote_service->service_characteristics[index].client_config_handle = 0;         /*client config handles*/
	 cymote_service->service_characteristics[index].server_config_handle = 0;         /*server config handles*/
	 cymote_service->service_characteristics[index].presentation_format = NULL;
	 
 }

 /* Register cymote_service as primary service instance inside stack. */
 at_ble_status_t cymote_primary_service_define(cymote_service_handler_t *cymote_primary_service, at_ble_handle_t *chr_handle)
 {
	int i;
	printf("\r\n");
	for (i=0;i<16;i++){
		printf("%u\r\n", cymote_primary_service->uuid.uuid[i]);
	}
	printf("\r\n");

	at_ble_uuid_t service_uuid;
	service_uuid.type=AT_BLE_UUID_128;
	for(i=0;i<16;i++){
		service_uuid.uuid[i] = CYMOTE_SERVICE_UUID[i];
	}

	at_ble_uuid_t attribute_descripton_uuid;
	attribute_descripton_uuid.type = AT_BLE_UUID_128;
	for(i=0;i<16;i++){
		attribute_descripton_uuid.uuid[i] = 1;
	}

	at_ble_chr_t characteristics[1];
	at_ble_char_val_t characteristic_value; 
	at_ble_user_desc_t characteristic_description;
	at_ble_char_presentation_t *characteristic_presentation = NULL;
	at_ble_server_config_desc_t client_config;
	at_ble_server_config_desc_t server_config;
	at_ble_generic_att_desc_t attribute_description[1];



	characteristic_value.init_value = 123;
	characteristic_value.len = 3;
	characteristic_value.permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	characteristic_value.properties = AT_BLE_CHAR_READ;
	characteristic_value.uuid = cymote_primary_service->service_characteristics[0].uuid;

	characteristic_description.user_description = "Acceleration X";
	characteristic_description.len = 14;
	characteristic_description.permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;

	//characteristic_presentation = NULL;

	client_config.perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;

	server_config.perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;

	attribute_description[0].desc_val_length = 14;
	attribute_description[0].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	attribute_description[0].uuid = attribute_descripton_uuid;

	characteristics[0].char_val = characteristic_value;
	characteristics[0].user_desc = characteristic_description;
	characteristics[0].presentation_format = characteristic_presentation;
	characteristics[0].client_config_desc = client_config;
	characteristics[0].server_config_desc = server_config;
	characteristics[0].additional_desc_list = attribute_description;
	characteristics[0].additional_desc_count = 1;



	at_ble_service_t cymote_service;
	cymote_service.type	= PRIMARY_SERVICE;
	cymote_service.uuid = service_uuid;
	cymote_service.perm	= AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	cymote_service.inc_list = NULL;
	cymote_service.included_count = 0;
	cymote_service.char_list = characteristics;
	cymote_service.char_count = 1;

	/*
	 return(at_ble_primary_service_define(&cymote_primary_service->uuid,
	 &cymote_primary_service->service_handle,
	 NULL, 0,
	 cymote_primary_service->service_characteristics, MAX_NUM_CHARACTERISTICS));
	 */

	 at_ble_status_t status_of_service_define = at_ble_service_define(&cymote_service);
	 *chr_handle = cymote_service.char_list[0].char_val.handle;
	 return(status_of_service_define);
 }

 /* Initialize a service and return handles to its characteristics so they can be updated. */
 at_ble_status_t cymote_service_init(at_ble_service_t *cymote_service, cymote_characteristic_handle_t *cymote_handles){
	int i;
	at_ble_status_t status = AT_BLE_FAILURE;
	at_ble_uuid_t service_uuid;
	at_ble_uuid_t characteristic_uuids[MAX_NUM_CHARACTERISTICS];
	at_ble_chr_t characteristics[MAX_NUM_CHARACTERISTICS];
	at_ble_char_val_t characteristic_values[MAX_NUM_CHARACTERISTICS];
	at_ble_user_desc_t characteristic_description[MAX_NUM_CHARACTERISTICS];
	at_ble_char_presentation_t *characteristic_presentation = NULL;
	at_ble_server_config_desc_t client_config[MAX_NUM_CHARACTERISTICS];
	at_ble_server_config_desc_t server_config[MAX_NUM_CHARACTERISTICS];
	
	/* an array of descriptions in needed for the function. This implementation has NUMBER_ATTRIBUTE_DESCRIPTIONS 
	 * for each characteristic.
	 */
	at_ble_generic_att_desc_t attribute_descriptions[MAX_NUM_CHARACTERISTICS][NUMBER_ATTRIBUTE_DESCRIPTIONS];

	
	//setup the characteristic uuids
	characteristic_uuid_init(characteristic_uuids);
	DBG_LOG("made it past characteristic uuid init");

	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		characteristic_values[i].init_value = 00000000;
		characteristic_values[i].len = 8;
		characteristic_values[i].permissions = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		characteristic_values[i].properties = AT_BLE_CHAR_READ;
		characteristic_values[i].uuid = characteristic_uuids[i];
	}
	DBG_LOG("made it past characteristic value loop");
	
	//initializes the characteristic descriptions
	characteristic_description_init(characteristic_description);
	DBG_LOG("made it past characteristic description init");
	fflush(stdout);
	
	//initialized to null. change it here if needed.
	//characteristic_presentation = NULL;

	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		client_config[i].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		server_config[i].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	}
	DBG_LOG("made it past server and client config");

	//initialize attribute descriptions
	characteristic_attribute_descriptions_init(attribute_descriptions);
	DBG_LOG("made it past characteristic attribute descriptions init");
	
	//assign characteristics their properties
	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		characteristics[i].char_val = characteristic_values[i];
		characteristics[i].user_desc = characteristic_description[i];
		characteristics[i].presentation_format = characteristic_presentation;
		characteristics[i].client_config_desc = client_config[i];
		characteristics[i].server_config_desc = server_config[i];
		characteristics[i].additional_desc_list = attribute_descriptions[i];
		characteristics[i].additional_desc_count = NUMBER_ATTRIBUTE_DESCRIPTIONS;
	}
	DBG_LOG("made it past assigning characteristics their properties");


	//setup the service uuid
	service_uuid.type=AT_BLE_UUID_128;
	for(i=0;i<16;i++){
		service_uuid.uuid[i] = CYMOTE_SERVICE_UUID[i];
	}
	DBG_LOG("made it past service uuid init");

	/* wrap everything into the service */
	cymote_service->type	       = PRIMARY_SERVICE;
	cymote_service->uuid           = service_uuid;
	cymote_service->perm	       = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
	cymote_service->inc_list       = NULL;    //no secondary services included
	cymote_service->included_count = 0;       //no secondary services included
	cymote_service->char_list      = characteristics;
	cymote_service->char_count     = MAX_NUM_CHARACTERISTICS;
	DBG_LOG("made it past wrapping service");

	status = at_ble_service_define(cymote_service);
	DBG_LOG("made it past service define");

	//assign the address of the handles so characteristics can be updated
	cymote_handles->accel_x_handle    = cymote_service->char_list[0].char_val.handle;
	cymote_handles->accel_y_handle    = cymote_service->char_list[1].char_val.handle;
	cymote_handles->accel_z_handle    = cymote_service->char_list[2].char_val.handle;
	cymote_handles->gyro_x_handle     = cymote_service->char_list[3].char_val.handle;
	cymote_handles->gyro_y_handle     = cymote_service->char_list[4].char_val.handle;
	cymote_handles->gyro_z_handle     = cymote_service->char_list[5].char_val.handle;
	cymote_handles->magnet_x_handle   = cymote_service->char_list[6].char_val.handle;
	cymote_handles->magnet_y_handle   = cymote_service->char_list[7].char_val.handle;
	cymote_handles->magnet_z_handle   = cymote_service->char_list[8].char_val.handle;
	cymote_handles->joystick_x_handle = cymote_service->char_list[9].char_val.handle;
	cymote_handles->joystick_y_handle = cymote_service->char_list[10].char_val.handle;
	cymote_handles->buttons_handle    = cymote_service->char_list[11].char_val.handle;
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
		characteristic_uuids[0].uuid[i] = ACCEL_X_UUID[i];
		characteristic_uuids[1].uuid[i] = ACCEL_Y_UUID[i];
		characteristic_uuids[2].uuid[i] = ACCEL_Z_UUID[i];
		characteristic_uuids[3].uuid[i] = GYRO_X_UUID[i];
		characteristic_uuids[4].uuid[i] = GYRO_Y_UUID[i];
		characteristic_uuids[5].uuid[i] = GYRO_Z_UUID[i];
		characteristic_uuids[6].uuid[i] = MAGNET_X_UUID[i];
		characteristic_uuids[7].uuid[i] = MAGNET_Y_UUID[i];
		characteristic_uuids[8].uuid[i] = MAGNET_Z_UUID[i];
		characteristic_uuids[9].uuid[i] = JOYSTICK_X_UUID[i];
		characteristic_uuids[10].uuid[i] = JOYSTICK_Y_UUID[i];
		characteristic_uuids[11].uuid[i] = BUTTONS_UUID[i];
	}
 }

 /* Initializes the characteristic description list */
 void characteristic_description_init(at_ble_user_desc_t characteristic_descriptions[MAX_NUM_CHARACTERISTICS]){
	int i;

	const char* desc[MAX_NUM_CHARACTERISTICS] = {"Acceleration X",
											     "Acceleration Y",
											     "Acceleration Z",
											     "Gyroscope X",
											     "Gyroscope Y",
											     "Gyroscope Z",
											     "Magnetometer X",
											     "Magnetometer Y",
											     "Magnetometer Z",
											     "Joystick X",
											     "Joystick Y",
											     "Buttons"};

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
			descriptions[i][j].desc_val_length = 15;
			descriptions[i][j].perm = AT_BLE_ATTR_READABLE_NO_AUTHN_NO_AUTHR;
		}
	}

	//Use the data UUID but adding in a 1
	for(i=0;i<NUMBER_ATTRIBUTE_DESCRIPTIONS;i++){
		for(j=0;j<16;j++){
			descriptions[0][i].uuid.uuid[j] = ACCEL_X_UUID[j];
			descriptions[1][i].uuid.uuid[j] = ACCEL_Y_UUID[j];
			descriptions[2][i].uuid.uuid[j] = ACCEL_Z_UUID[j];
			descriptions[3][i].uuid.uuid[j] = GYRO_X_UUID[j];
			descriptions[4][i].uuid.uuid[j] = GYRO_Y_UUID[j];
			descriptions[5][i].uuid.uuid[j] = GYRO_Z_UUID[j];
			descriptions[6][i].uuid.uuid[j] = MAGNET_X_UUID[j];
			descriptions[7][i].uuid.uuid[j] = MAGNET_Y_UUID[j];
			descriptions[8][i].uuid.uuid[j] = MAGNET_Z_UUID[j];
			descriptions[9][i].uuid.uuid[j] = JOYSTICK_X_UUID[j];
			descriptions[10][i].uuid.uuid[j] = JOYSTICK_Y_UUID[j];
			descriptions[11][i].uuid.uuid[j] = BUTTONS_UUID[j];
		}
		descriptions[0][i].uuid.type = AT_BLE_UUID_128;
		descriptions[1][i].uuid.type = AT_BLE_UUID_128;
		descriptions[2][i].uuid.type = AT_BLE_UUID_128;
		descriptions[3][i].uuid.type = AT_BLE_UUID_128;
		descriptions[4][i].uuid.type = AT_BLE_UUID_128;
		descriptions[5][i].uuid.type = AT_BLE_UUID_128;
		descriptions[6][i].uuid.type = AT_BLE_UUID_128;
		descriptions[7][i].uuid.type = AT_BLE_UUID_128;
		descriptions[8][i].uuid.type = AT_BLE_UUID_128;
		descriptions[9][i].uuid.type = AT_BLE_UUID_128;
		descriptions[10][i].uuid.type = AT_BLE_UUID_128;
		descriptions[11][i].uuid.type = AT_BLE_UUID_128;
	}
	for(i=0;i<MAX_NUM_CHARACTERISTICS;i++){
		for(j=0;j<NUMBER_ATTRIBUTE_DESCRIPTIONS;j++){
		descriptions[i][j].uuid.uuid[13] = 0x10;
		}
	}
	
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

 