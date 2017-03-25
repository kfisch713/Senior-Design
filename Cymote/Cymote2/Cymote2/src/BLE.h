/*
 * BLE.h
 *
 * Created: 11/28/2016 8:22:39 PM
 *  Author: kfisc
 */ 


#ifndef BLE_H_
#define BLE_H_

#ifndef BLE_DEVICE_ROLE
#define BLE_DEVICE_ROLE BLE_ROLE_ALL
#endif

#include "platform.h"
#include "at_ble_api.h"
#include "ble_manager.h"
#include "timer_hw.h"



/* APP_FAST_ADV between 0x0020 and 0x4000 in 0.625 ms units (20ms to 10.24s). */
#define APP_FAST_ADV						(1600)

/* APP_ADV_TIMEOUT Advertising time-out between 0x0001 and 0x028F in seconds, 0x0000 disables time-out.*/
#define APP_ADV_TIMEOUT						(0)

#define BLE_UPDATE_INTERVAL					(1)

/* number of cymote service characteristics. See cymote_characteristic_value_t */
#define MAX_NUM_CHARACTERISTICS 12


#define ACCEL_MAX_LEN       (0x0f)
#define ACCEL_LEN           (0x03)
#define ACCEL_DEFAULT       ("123")
#define GYRO_MAX_LEN        ACCEL_MAX_LEN
#define GYRO_LEN            ACCEL_LEN
#define GYRO_DEFAULT        ACCEL_DEFAULT
#define MAGNET_MAX_LEN      ACCEL_MAX_LEN
#define MAGNET_LEN          ACCEL_LEN
#define MAGNET_DEFAULT      ACCEL_DEFAULT
#define JOYSTICK_MAX_LEN    ACCEL_MAX_LEN
#define JOYSTICK_LEN        ACCEL_LEN
#define JOYSTICK_DEFAULT    ACCEL_DEFAULT
#define BUTTONS_MAX_LEN     (0x0f)
#define BUTTONS_LEN         (0x08)
#define BUTTONS_DEFAULT     ("00000000")


typedef struct {
	at_ble_uuid_t uuid;
	at_ble_handle_t service_handle;
	at_ble_characteristic_t service_characteristics[MAX_NUM_CHARACTERISTICS];
} cymote_service_handler_t;


/* characteristic value information */
typedef struct {
	//accelerometer data x axis
	uint8_t accel_x[ACCEL_MAX_LEN];
	//accelerometer data y axis
	uint8_t accel_y[ACCEL_MAX_LEN];
	//accelerometer data z axis
	uint8_t accel_z[ACCEL_MAX_LEN];
	//gyroscope data x axis
	uint8_t gyro_x[GYRO_MAX_LEN];
	//gyroscope data y axis
	uint8_t gyro_y[GYRO_MAX_LEN];
	//gyroscope data z axis
	uint8_t gyro_z[GYRO_MAX_LEN];
	//magnetometer data x axis
	uint8_t magnet_x[MAGNET_MAX_LEN];
	//magnetometer data y axis
	uint8_t magnet_y[MAGNET_MAX_LEN];
	//magnetometer data z axis
	uint8_t magnet_z[MAGNET_MAX_LEN];
	//joystick data x axis
	uint8_t joystick_x[JOYSTICK_MAX_LEN];
	//joystick data y axis
	uint8_t joystick_y[JOYSTICK_MAX_LEN];
	//buttons data
	uint8_t buttons[BUTTONS_MAX_LEN];	
}cymote_characteristic_value_t;

/* Need one entry for every element in cymote_characteristic_value_t. Order needs to match too. */
typedef enum {
	CYMOTE_ACCEL_X = 0,
	CYMOTE_ACCEL_Y,
	CYMOTE_ACCEL_Z,
	CYMOTE_GYRO_X,
	CYMOTE_GYRO_Y,
	CYMOTE_GYRO_Z,
	CYMOTE_MAGNET_X,
	CYMOTE_MAGNET_Y,
	CYMOTE_MAGNET_Z,
	CYMOTE_JOYSTICK_X,
	CYMOTE_JOYSTICK_Y,
	CYMOTE_BUTTONS,
} cymote_info_type;

/* Configurable client characteristic data for a given cymote info type*/
typedef struct{
	// length of the data to be updated
	uint16_t data_len;
	// data to be updated
	uint8_t *info_data;
}cymote_info_data;

/************** Macros **************/

/* Macro used for updating all three axes of the accelerometer after defining the service using cymote_primary_service_define*/
#define UPDATE_ACCEL(ptr, info_data_x, info_data_y, info_data_z, conn_handle) do {   \
	if (cymote_info_update(ptr, CYMOTE_ACCEL_X, info_data_x, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating accelerometer x axis raw data failed\r\n");  \
	}\
	if (cymote_info_update(ptr, CYMOTE_ACCEL_Y, info_data_y, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating accelerometer y axis raw data failed\r\n");  \
	}\
	if (cymote_info_update(ptr, CYMOTE_ACCEL_Z, info_data_z, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating accelerometer z axis raw data failed\r\n");  \
	}\
} while (0)

/* Macro used for updating all three axes of the gyroscope after defining the service using cymote_primary_service_define*/
#define UPDATE_GYRO(ptr, info_data_x, info_data_y, info_data_z, conn_handle) do {   \
	if (cymote_info_update(ptr, CYMOTE_GYRO_X, info_data_x, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating gyroscope x axis raw data failed\r\n");  \
	}\
	if (cymote_info_update(ptr, CYMOTE_GYRO_Y, info_data_y, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating gyroscope y axis raw data failed\r\n");  \
	}\
	if (cymote_info_update(ptr, CYMOTE_GYRO_Z, info_data_z, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating gyroscope z axis raw data failed\r\n");  \
	}\
} while (0)

/* Macro used for updating all three axes of the magnetometer after defining the service using cymote_primary_service_define*/
#define UPDATE_MAGNET(ptr, info_data_x, info_data_y, info_data_z, conn_handle) do {   \
	if (cymote_info_update(ptr, CYMOTE_MAGNET_X, info_data_x, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating magnetometer x axis raw data failed\r\n");  \
	}\
	if (cymote_info_update(ptr, CYMOTE_MAGNET_Y, info_data_y, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating magnetometer y axis raw data failed\r\n");  \
	}\
	if (cymote_info_update(ptr, CYMOTE_MAGNET_Z, info_data_z, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating magnetometer z axis raw data failed\r\n");  \
	}\
} while (0)

/* Macro used for updating the two axes of the joystick after defining the service using cymote_primary_service_define*/
#define UPDATE_JOYSTICK(ptr, info_data_x, info_data_y, conn_handle) do {   \
	if (cymote_info_update(ptr, CYMOTE_JOYSTICK_X, info_data_x, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating accelerometer x axis raw data failed\r\n");  \
	}\
	if (cymote_info_update(ptr, CYMOTE_JOYSTICK_Y, info_data_y, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating accelerometer y axis raw data failed\r\n");  \
	}\
} while (0)

/* Macro used for updating the buttons after defining the service using cymote_primary_service_define*/
#define UPDATE_BUTTONS(ptr, info_data, conn_handle) do {   \
	if (cymote_info_update(ptr, CYMOTE_BUTTONS, info_data, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating button data failed\r\n");  \
	}\
} while (0)





/* Prototypes */
void cymote_init_service(cymote_service_handler_t* cymote_service);
at_ble_status_t cymote_primary_service_define(cymote_service_handler_t *cymote_primary_service, at_ble_handle_t *chr_handle);
at_ble_status_t cymote_info_update(cymote_service_handler_t *cymote_serv , cymote_info_type info_type, cymote_info_data* info_data, at_ble_handle_t conn_handle);
at_ble_status_t device_information_advertise(void);
at_ble_status_t ble_paired_app_event(void *param);
at_ble_status_t ble_disconnected_app_event(void *param);
at_ble_status_t ble_connected_app_event(void *param);




#endif /* BLE_H_ */