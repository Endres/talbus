#include "talbus.h"
#include "mcp2515.h"

void talbus_send(talbus_message* message) {
  can_message msg;
  msg.SIDH = *(uint8_t*)message;
  msg.SIDL = *((uint8_t*)message+1) & 0xE0 | TXBSIDL_EXIDE;
  if(message->data_length > 0) {
    msg.EID8 = message->data[0];
  }
  if(message->data_length > 1) {
    msg.EID0 = message->data[1];
  }
  if(message->data_length > 2) {
    if(message->data_length > 10) {
      message->data_length = 10;
    }
    msg.DLC = message->data_length - 2;
    for(uint8_t i = 2; i < message->data_length; i++) {
      msg.data[i-2] = message->data[i];
    }
  }
  mcp2515_queue(msg);
}

uint8_t talbus_check(talbus_message* message) {
  if(rx_count > 0) {
    /* ... check if special message,... parse into pointer if standard message and return */
  }
}
