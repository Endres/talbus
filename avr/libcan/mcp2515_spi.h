#include <stdint.h>

#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define LED_PIN  PB1
#define CS_PIN   PB2
#define MOSI_PIN PB3
#define MISO_PIN PB4
#define SCK_PIN  PB5

#define SPI_RESET       0xC0
#define SPI_READ        0x03
#define SPI_READBUFFER  0x90
#define SPI_WRITE       0x02
#define SPI_LOADBUFFER  0x40
#define SPI_RTS         0x80
#define SPI_READ_STATUS 0xA0
#define SPI_RX_STATUS   0xB0
#define SPI_BIT_MODIFY  0x05

void spi_init();
uint8_t spi_send(uint8_t data);

#define SPI_ENABLE()  SPI_PORT &= ~(1 << CS_PIN)
#define SPI_DISABLE() SPI_PORT |= (1 << CS_PIN)
