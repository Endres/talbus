#ifndef _TALBUS_H_
 #define _TALBUS_H_

#define SC1 0x02
#define SC0 0x01

typedef struct {
  uint8_t VID:3;
  uint8_t MID:6;
  uint8_t DID:2;
  uint8_t data_length;
  uint8_t *data;
} talbus_message;

void talbus_send(talbus_message message);

#endif
