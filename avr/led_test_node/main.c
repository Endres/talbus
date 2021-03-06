#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include <talbus.h>
#include <mcp2515.h>

uint8_t led_status2 = 0;
uint16_t uptime = 0;
uint8_t uptime_5 = 0;

int main() {
	DDRB |= _BV(PB1);

	/* initialize timers */
	OCR1A = 0x30D3;
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	TCCR1B |= (1 << CS12);
	sei();

	mcp2515_init();

	talbus_message message;
	while(1) {
		mcp2515_loop();

		if(talbus_receive(&message) != NULL) {
			if(message.address == CONFIG_ADDRESS && message.direction == DIR_TO_ADDRESS) {
				if(message.protocol == PROTO_ROOM) {
					if(message.data[0] == FUNC_ROOM_LIGHT_ON) {
						if(message.data[1] == 0x00) {
							led_status2 = 1;
							PORTB |= _BV(PB1);
						}
					} else if(message.data[0] == FUNC_ROOM_LIGHT_OFF) {
						if(message.data[1] == 0x00) {
							led_status2 = 0;
							PORTB &= ~_BV(PB1);
						}
					}
				}
			}
		}

		if(uptime_5 >= 5) {
			uptime += uptime_5/5;
			uptime_5 %= 5;
			if(uptime % 60 == 0) {
				talbus_message msg;
				msg.address = CONFIG_ADDRESS;
				msg.protocol = PROTO_ROOM;
				msg.direction = DIR_FROM_ADDRESS;
				msg.data_length = 4;
				msg.data[0] = FUNC_ROOM_LIGHT_DIM;
				msg.data[1] = 0x01;
				msg.data[2] = (uint8_t)(uptime >> 8);
				msg.data[3] = (uint8_t)uptime;
				talbus_send(&msg);
			}
		}
	}
}

ISR(TIMER1_COMPA_vect) {
	if(led_status2 == 0) {
		PORTB &= ~_BV(PB1); /* Disable debug LED */
	}
	uptime_5++;
}
