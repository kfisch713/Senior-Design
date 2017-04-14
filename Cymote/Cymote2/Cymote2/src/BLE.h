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
#define TIMER_UPDATE_IN_MS					(7) // 7 milliseconds

/* number of cymote service characteristics. See cymote_characteristic_value_t */
#define MAX_NUM_CHARACTERISTICS 5

/* Number of attribute descriptions. Used in initializing characteristics. */
#define NUMBER_ATTRIBUTE_DESCRIPTIONS 1


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

typedef struct{
	at_ble_handle_t accel_handle;
	at_ble_handle_t gyro_handle;
	at_ble_handle_t magnet_handle;
	at_ble_handle_t joystick_buttons_handle;
	at_ble_handle_t time_handle;
} cymote_characteristic_handle_t;

typedef struct{
	at_ble_uuid_t service_uuid;
	at_ble_uuid_t characteristic_uuids[MAX_NUM_CHARACTERISTICS];
	at_ble_chr_t characteristics[MAX_NUM_CHARACTERISTICS];
	at_ble_char_val_t characteristic_values[MAX_NUM_CHARACTERISTICS];
	at_ble_user_desc_t characteristic_description[MAX_NUM_CHARACTERISTICS];
	at_ble_char_presentation_t characteristic_presentation[MAX_NUM_CHARACTERISTICS];
	at_ble_server_config_desc_t client_config[MAX_NUM_CHARACTERISTICS];
	at_ble_server_config_desc_t server_config[MAX_NUM_CHARACTERISTICS];

	/* an array of descriptions in needed for the function. This implementation has NUMBER_ATTRIBUTE_DESCRIPTIONS 
	 * for each characteristic.
	 */
	at_ble_generic_att_desc_t attribute_descriptions[MAX_NUM_CHARACTERISTICS][NUMBER_ATTRIBUTE_DESCRIPTIONS];
}cymote_service_data_t;


/* Prototypes */
at_ble_status_t device_information_advertise(void);
at_ble_status_t ble_paired_app_event(void *param);
at_ble_status_t ble_disconnected_app_event(void *param);
at_ble_status_t ble_connected_app_event(void *param);

at_ble_status_t cymote_service_init(at_ble_service_t *cymote_service, cymote_service_data_t* data, cymote_characteristic_handle_t *cymote_handles);
at_ble_status_t cymote_service_define(at_ble_service_t *service);
void characteristic_uuid_init(at_ble_uuid_t characteristic_uuids[MAX_NUM_CHARACTERISTICS]);
void characteristic_description_init(at_ble_user_desc_t characteristic_descriptions[MAX_NUM_CHARACTERISTICS]);
void characteristic_attribute_descriptions_init(at_ble_generic_att_desc_t descriptions[MAX_NUM_CHARACTERISTICS][NUMBER_ATTRIBUTE_DESCRIPTIONS]);




#endif /* BLE_H_ */