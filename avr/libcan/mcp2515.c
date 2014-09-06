#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "mcp2515_spi.h"
#include "mcp2515.h"
#include "talbus.h"

void mcp2515_init() {
  /* Setup SPI */
  spi_init();
  
  /* Reset MCP2515 */
  SPI_ENABLE();
  spi_send(SPI_RESET);
  SPI_DISABLE();
  
  /* (maybe!) Wait some cycles */
  /*for(uint8_t i = 0; i < 64; i++) {
    __asm__("nop");
  }*/
  
  /* Configure Clock */
  #if F_MCP == 8000000
    #define CNF1_T 0x01
  #elif F_MCP == 16000000
    #define CNF1_T 0x03
  #elif F_MCP == 20000000
    #define CNF1_T 0x04
  #else
    #error CAN-Baudrate is only defined for 8, 16 and 20 MHz
  #endif
  mcp2515_write_open(CNF3);
  spi_send(0x05); /* PHSEG = 5 */
  spi_send(0xF1); /* BTLMODE, SAM, PHSEG1 = 6, PRSEG = 1 */
  spi_send(0x80 | CNF1_T); /* SJW = 2 * TQ */
  
  /* Configure interrupt */
  spi_send(CANINT_RX0I | CANINT_RX1I); /* CANINTE = RX0IE | RX1IE */
  SPI_DISABLE();
  EICRA |= (1 << ISC01);
  EIMSK |= (1 << INT0);
  sei();
  
  /* Configure RX Control */
  mcp2515_write(RXB0CTRL, RXBCTRL_RXM1 | RXBCTRL_RXM0 | RXB0CTRL_BUKT);
  
  /* Set additional I/O pins and switch to normal mode */
  mcp2515_write_open(BFPCTRL);
  spi_send(BFPCTRL_B0BFE | BFPCTRL_B1BFE);
  spi_send(TXRTSCTRL_B0RTSM | TXRTSCTRL_B1RTSM | TXRTSCTRL_B2RTSM);
  spi_send(0x00);
  spi_send(0x00); /* Switch to normal operation mode */
  SPI_DISABLE();
}

uint8_t mcp2515_get_input() {
  return (mcp2515_read(TXRTSCTRL) >> 3);
}

void mcp2515_set_output(uint8_t mask, uint8_t output) {
  mcp2515_bit_modify(BFPCTRL, (mask << 4) & (BFPCTRL_B0BFS | BFPCTRL_B1BFS),
    (output << 4) & (BFPCTRL_B0BFS | BFPCTRL_B1BFS));
}

uint8_t mcp2515_read(uint8_t address) {
  uint8_t data;
  SPI_ENABLE();
  spi_send(SPI_READ);
  spi_send(address);
  data = spi_send(0x00);
  SPI_DISABLE();
  return data;
}

void mcp2515_multi_read(uint8_t address, uint8_t* buf, uint8_t length) {
  SPI_ENABLE();
  spi_send(SPI_READ);
  spi_send(address);
  for(uint8_t i = 0; i < length; i++) {
    *(buf++) = spi_send(0x00);
  }
  SPI_DISABLE();
}

void mcp2515_read_rx_buffer(uint8_t spi_buffer, uint8_t* buf, uint8_t length) {
  SPI_ENABLE();
  spi_send(SPI_READBUFFER | spi_buffer);
  for(uint8_t i = 0; i < length; i++) {
    *(buf++) = spi_send(0x00);
  }
  SPI_DISABLE();
}

void mcp2515_write(uint8_t address, uint8_t data) {
  SPI_ENABLE();
  spi_send(SPI_WRITE);
  spi_send(address);
  spi_send(data);
  SPI_DISABLE();
}

void mcp2515_multi_write(uint8_t address, uint8_t* data, uint8_t length) {
  SPI_ENABLE();
  spi_send(SPI_WRITE);
  spi_send(address);
  for(uint8_t i = 0; i < length; i++) {
    spi_send(*(data++));
  }
  SPI_DISABLE();
}

void mcp2515_write_open(uint8_t address) {
  SPI_ENABLE();
  spi_send(SPI_WRITE);
  spi_send(address);
}

void mcp2515_load_tx_buffer(uint8_t spi_buffer) {
  SPI_ENABLE();
  spi_send(SPI_LOADBUFFER | spi_buffer);
}

/* void mcp2515_rts(uint8_t spi_buffer); */

uint8_t mcp2515_read_status() {
  uint8_t data;
  SPI_ENABLE();
  spi_send(SPI_READ_STATUS);
  data = spi_send(0x00);
  SPI_DISABLE();
  return data;
}

uint8_t mcp2515_rx_status() {
  uint8_t data;
  SPI_ENABLE();
  spi_send(SPI_RX_STATUS);
  data = spi_send(0x00);
  SPI_DISABLE();
  return data;
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
  SPI_ENABLE();
  spi_send(SPI_BIT_MODIFY);
  spi_send(address);
  spi_send(mask);
  spi_send(data);
  SPI_DISABLE();
}

uint8_t rx_count = 0, tx_count = 0;
uint8_t rx_first = 0, tx_first = 0;
can_message rx_buffer[RX_BUFFER_COUNT], tx_buffer[TX_BUFFER_COUNT];

ISR(INT0_vect) {
  mcp2515_rx_loop();
}

void mcp2515_queue(can_message msg) {
  while(tx_count >= TX_BUFFER_COUNT) {
    mcp2515_loop();
  }
  tx_buffer[(tx_first + tx_count) % TX_BUFFER_COUNT] = msg;
  tx_count++;
}

can_message* mcp2515_receive(can_message* dest) {
  if(rx_count == 0) {
    return NULL;
  }

  *dest = rx_buffer[rx_first];

  rx_count--;
  if(rx_count == 0) {
    rx_first = 0;
  } else {
    rx_first = (rx_first + 1) % RX_BUFFER_COUNT;
  }

  return dest;
}

uint8_t led_status = 0;

void mcp2515_rx_loop() {
  uint8_t rx_status = mcp2515_rx_status();
  uint8_t buffer;
  if(led_status > 0) {
    led_status++;
    if(led_status == 0) {
      SPI_PORT &= ~(1 << LED_PIN);
    }
  }
  
  if(rx_status & 0x40) {
    buffer = SPI_RXB0SIDH;
  } else if(rx_status & 0x80) {
    buffer = SPI_RXB1SIDH;
  } else {
    return; /* No message in RX buffers */
  }
  if(led_status == 0) {
    SPI_PORT |= LED_PIN;
  }
  led_status = 1;
  if(rx_count < RX_BUFFER_COUNT) {
    mcp2515_read_rx_buffer(buffer, (uint8_t*)rx_buffer+((rx_first + rx_count) % RX_BUFFER_COUNT), 13);
    rx_count++;
  }
}

void mcp2515_loop() {
  uint8_t status, buffer = -1;
  if(tx_count > 0) {
    status = mcp2515_read_status();
    if(status & 0x04 == 0x00) {
      buffer = SPI_TXB0SIDH;
    } else if(status & 0x10 == 0x00) {
      buffer = SPI_TXB1SIDH;
    } else if(status & 0x40 == 0x00) {
      buffer = SPI_TXB2SIDH;
    }
    if(buffer != -1) { /* free TX buffer found */
      mcp2515_load_tx_buffer(buffer);
      for(uint8_t j = 0; j < 5 + (tx_buffer[tx_first].DLC & 0x07); j++) {
        spi_send(*((uint8_t*)(tx_buffer+tx_first)+j));
      }
      SPI_DISABLE();
      mcp2515_write(0x30 + buffer * 0x08, /* Calculate Address of TXBnCTRL */
        TXBCTRL_TXREQ | TXBCTRL_TXP0); /* Mid-Low priority */
      tx_count--;
      if(tx_count == 0) {
        tx_first = 0;
      } else {
        tx_first = (tx_first + 1) % TX_BUFFER_COUNT;
      }
    }
  }
  
  mcp2515_rx_loop();
}
