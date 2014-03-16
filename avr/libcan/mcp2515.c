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
  //TODO: Configure AVR Interrupt?
  
  /* Configure Filters */
  mcp2515_write_open(RXF0SIDH);
  spi_send(NODE_ID);
  spi_send(RXBSIDL_IDE | SC1);
  spi_send(0x00);
  spi_send(0x00);
  spi_send(NODE_ID);
  spi_send(RXBSIDL_IDE | SC0);
  spi_send(0x00);
  spi_send(0x00);
  SPI_DISABLE();
  mcp2515_write_open(RXM0SIDH);
  spi_send(0xFF);
  spi_send(RXBSIDL_IDE | SC1 | SC0);
  spi_send(0x00);
  spi_send(0x00);
  SPI_DISABLE();
  
  #ifdef CONFIGURE_FILTERS()
    CONFIGURE_FILTERS();
  #else
    #warning No RX-Filters in Receive Buffer 1 configured. Device will only receive special CAN-Messages!
  #endif
  
  /* Configure RX Control */
  mcp2515_write(RXB0CTRL, RXB0CTRL_BUKT); /* so okay? */
  
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

extern uint8_t rx_counter = 0, tx_counter = 0;
extern can_message rx_buffer[RX_BUFFER_COUNT], tx_buffer[TX_BUFFER_COUNT];

ISR(/*TODO: Interrupt-Name */) {
  uint8_t rx_status = mcp2515_rx_status();
  if(rx_status & 0xC0) {
    /* in which buffer,...? */
    if(rx_counter < RX_BUFFER_COUNT) {
      /*TODO: Fill buffer */
      rx_counter++;
    }
  }
}

void mcp2515_queue(can_message msg) {
  if(tx_counter < TX_BUFFER_COUNT) {
    /*TODO: Find free slot tx_buffer[i]... Implement real queue */
    /* If no free slot available, loop mcp2515_loop() which is otherwise called by the overlaying loop function to not lose the message */
    tx_buffer[i] = msg; 
    tx_counter++;
  }
}

void mcp2515_loop() {
  uint8_t status, buffer = 255;
  if(tx_counter > 0) {
    /*TODO: Find message to send, tx_buffer[i] */
    status = mcp2515_read_status();
    if(status & 0x04 == 0x00) {
      buffer = SPI_TXB0SIDH;
    } else if(status & 0x10 == 0x00) {
      buffer = SPI_TXB1SIDH;
    } else if(status & 0x40 == 0x00) {
      buffer = SPI_TXB2SIDH;
    }
    if(buffer == 255) return;
    
    mcp2515_load_tx_buffer(buffer);
    for(uint8_t j = 0; j < 5 + (tx_buffer[i].DLC & 0x07); j++) {
      spi_send(*((*uint8_t)(tx_buffer+i)+j));
    }
    SPI_DISABLE();
    mcp2515_write(0x30 + buffer * 0x08, /* Calculate Address of TXBnCTRL */
      TXBCTRL_TXREQ | TXBCTRL_TXP0); /* Mid-Low priority */
    /*TODO: Free message tx_buffer[i] */
  }
}
