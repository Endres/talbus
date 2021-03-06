#include <avr/io.h>
#include "mcp2515_spi.h"

void spi_init() {
  SPI_DDR |= (1 << CS_PIN) | (1 << MOSI_PIN) | (1 << SCK_PIN) | (1 << LED_PIN);
  SPI_PORT |= (1 << CS_PIN);
  SPCR = (1 << SPE) | (1 << MSTR);
  SPSR = (1 << SPI2X);
}

uint8_t spi_send(uint8_t data) {
  SPDR = data;
  while(!(SPSR & (1 << SPIF)));
  return SPDR;
}
