/*
 * cymote.h
 *
 * Created: 10/31/2016 6:55:43 PM
 *  Author: kfisc
 */ 


#ifndef CYMOTE_H_
#define CYMOTE_H_

#include "platform.h"
#include "at_ble_api.h"
#include "ble_manager.h"

/* number of cymote service characteristics. See cymote_characteristic_value_t */
#define MAX_NUM_CHARACTERISTICS 0x03


#define BOARD_NAME_MAX_LEN     (0x14)
#define BOARD_NAME_LEN         (0x06)
#define CYMOTE_NAME			   ("CyMote")

#define DUMMY_DATA_MAX_LEN     (0x0f)
#define DUMMY_DATA_LEN         (0x01)
#define DUMMY_DATA_DEFAULT     ("A")

#define DUMMY_DATA_2_MAX_LEN   (0x0f)
#define DUMMY_DATA_2_LEN       (0x05)
#define DUMMY_DATA_2_DEFAULT   ("0.000")

typedef struct {
	at_ble_uuid_t uuid;
	at_ble_handle_t service_handle;
	at_ble_characteristic_t service_characteristics[MAX_NUM_CHARACTERISTICS];
} cymote_service_handler_t;


/* characteristic value information */
typedef struct {
	/// manufacturer name
	uint8_t board_name[BOARD_NAME_MAX_LEN];
	//dummy data
	uint8_t* dummy;
	//dummy data 2
	uint8_t* dummy2;
}cymote_characteristic_value_t;

/* Need one entry for every element in cymote_characteristic_value_t. Order needs to match too. */
typedef enum {
	CYMOTE_DEVICE_NAME = 0,
	CYMOTE_DUMMY_DATA,
	CYMOTE_DUMMY_DATA_2,
} cymote_info_type;

/* Configurable client characteristic data for a given cymote info type*/
typedef struct{
	// length of the data to be updated
	uint16_t data_len;
	// data to be updated
	uint8_t *info_data;
}cymote_info_data;

/* Macros */
/* Macro used for updating dummy data after defining the service using cymote_primary_service_define*/
#define UPDATE_DUMMY_DATA(ptr, info_data, conn_handle) do {   \
	if (cymote_info_update(ptr, CYMOTE_DUMMY_DATA, info_data, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating dummy data failed\r\n");  \
	}\
} while (0)

/* Macro used for updating dummy data 2 after defining the service using cymote_primary_service_define*/
#define UPDATE_DUMMY_DATA_2(ptr, info_data, conn_handle) do {   \
	if (cymote_info_update(ptr, CYMOTE_DUMMY_DATA_2, info_data, conn_handle) != AT_BLE_SUCCESS) { \
		DBG_LOG("Updating dummy data 2 failed\r\n");  \
	}\
} while (0)


/* Prototypes */
void cymote_init_service(cymote_service_handler_t* cymote_service);
at_ble_status_t cymote_primary_service_define(cymote_service_handler_t *cymote_primary_service);
at_ble_status_t cymote_info_update(cymote_service_handler_t *cymote_serv , cymote_info_type info_type, cymote_info_data* info_data, at_ble_handle_t conn_handle);

#endif /* CYMOTE_H_ */