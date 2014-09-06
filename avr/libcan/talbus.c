#include <avr/wdt.h>
#include <stdlib.h>
#include "talbus.h"
#include "mcp2515.h"
#include "../led_test_node/config.h"

void talbus_send(talbus_message* message) {
  can_message msg;

  msg.SIDH = message->address;
  msg.SIDL = ((message->protocol & 0x07) << 5) | TXBSIDL_EXIDE | ((message->direction & 0x01) << 1);

  if(message->data_length > 0) {
    msg.EID8 = message->data[0];
    if(message->data_length > 1) {
      msg.EID0 = message->data[1];
      if(message->data_length > 2) {
        if(message->data_length > 10) {
          message->data_length = 10;
        }
        msg.DLC = message->data_length - 2;
        for(uint8_t i = 2; i < message->data_length; i++) {
          msg.data[i-2] = message->data[i];
        }
      }
    }
  }

  mcp2515_queue(msg);
}

talbus_message* talbus_receive(talbus_message* message) {
  while(1) { /* Loop until return */
    can_message msg;

    if(mcp2515_receive(&msg) == NULL) {
      return NULL;
    }

    message->address = msg.SIDH;
    message->protocol = msg.SIDL >> 5;
    message->direction = (msg.SIDL >> 1) & 0x01;
    message->data_length = 2 + msg.DLC;
    if(message->data_length > 10) {
      message->data_length = 10;
    }
    message->data[0] = msg.EID8;
    message->data[1] = msg.EID0;
    for(uint8_t i = 2; i < message->data_length; i++) {
      message->data[i] = msg.data[i-2];
    }

    if(message->protocol == PROTO_MGMT) { /* Handle management protocol static */
      if(message->address == CONFIG_ADDRESS && message->direction == DIR_TO_ADDRESS) {
        uint8_t dest_address;
        switch(message->data[0]) {
          case FUNC_MGMT_PING:
            dest_address = message->data[1];
            message->data[1] = message->address;
            message->address = dest_address;
            message->data[0] = FUNC_MGMT_PONG;
            talbus_send(message);
            break;
          case FUNC_MGMT_RESET:
            wdt_enable(0);
            while(1);
            break;
          case FUNC_MGMT_FLASH:
            /* TODO: Make sure this is only available in bootloader... Decisions should be made */
            break;
        }
      }
    } else {
      return message;
    }
  }
}
