#ifndef _TALBUS_H_
 #define _TALBUS_H_

#include <stdint.h>

typedef enum {
	DIR_FROM_ADDRESS	= 0,
	DIR_TO_ADDRESS		= 1
} talbus_direction;

typedef enum {
	PROTO_MGMT			= 0,
	PROTO_ROOM			= 1,
	PROTO_MULTIMEDIA	= 2,
	PROTO_MESSAGE		= 3,
	PROTO_DEBUG			= 4
} talbus_protocols;

typedef struct {
	uint8_t address;
	talbus_protocols protocol;
	talbus_direction direction;
	uint8_t data_length;
	uint8_t data[10];
} talbus_message;

void talbus_init();
void talbus_send(talbus_message* message);
talbus_message* talbus_receive(talbus_message* message);

/* Functions of the different protocols */

typedef enum {
	FUNC_MGMT_PING			= 0x00,
	FUNC_MGMT_PONG			= 0x01,
	FUNC_MGMT_RESET			= 0x02,
	FUNC_MGMT_FLASH	 		= 0x03,
	FUNC_MGMT_FLASH_ACK		= 0x04
} talbus_proto_management;

typedef enum {
	FUNC_ROOM_LIGHT_OFF		= 0x00,
	FUNC_ROOM_LIGHT_ON		= 0x01,
	FUNC_ROOM_LIGHT_DIM		= 0x02,
	FUNC_ROOM_SWITCH_OFF	= 0x03,
	FUNC_ROOM_SWITCH_ON		= 0x04,
	FUNC_ROOM_SWITCH_DIM	= 0x05,
	FUNC_ROOM_STATUS_CLOSED	= 0x06,
	FUNC_ROOM_STATUS_OPENED	= 0x07,
	FUNC_ROOM_RELAIS_OFF	= 0x08,
	FUNC_ROOM_RELAIS_ON		= 0x09
} talbus_proto_room;

#endif
