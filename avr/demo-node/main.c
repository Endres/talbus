#include "config.h"
#include <talbus.h>
#include <mcp2515.h>

int main() {
	mcp2515_init();

	while(1) {
		mcp2515_loop();

		talbus_message message;
		if(talbus_receive(&message) != NULL) {
			/* message received */
		}
	}
}