#define BFPCTRL   0x0C
#define TXRTSCTRL 0x0D
#define CANSTAT   0x0E
#define CANCTRL   0x0F

#define TXRTSCTRL_B2RTS 0x20
#define TXRTSCTRL_B1RTS 0x10
#define TXRTSCTRL_B0RTS 0x08
#define TXRTSCTRL_B2RTSM 0x04
#define TXRTSCTRL_B1RTSM 0x02
#define TXRTSCTRL_B0RTSM 0x01

#define BFPCTRL_B1BFS 0x20
#define BFPCTRL_B0BFS 0x10
#define BFPCTRL_B1BFE 0x08
#define BFPCTRL_B0BFE 0x04
#define BFPCTRL_B1BFM 0x02
#define BFPCTRL_B0BFM 0x01

#define CANCTRL_REQOP2  0x80
#define CANCTRL_REQOP1  0x40
#define CANCTRL_REQOP0  0x20
#define CANCTRL_ABAT    0x10
#define CANCTRL_OSM     0x08
#define CANCTRL_CLKEN   0x04
#define CANCTRL_CLKPRE1 0x02
#define CANCTRL_CLKPRE0 0x01

#define CANSTAT_OPMOD2 0x80
#define CANSTAT_OPMOD1 0x40
#define CANSTAT_OPMOD0 0x20
#define CANSTAT_ICOD2  0x08
#define CANSTAT_ICOD1  0x04
#define CANSTAT_ICOD0  0x02

#define TEC 0x1C
#define REC 0x1D

#define CNF3    0x28
#define CNF2    0x29
#define CNF1    0x2A
#define CANINTE 0x2B
#define CANINTF 0x2C
#define EFLG    0x2D

#define CANINT_MERR 0x80
#define CANINT_WAKI 0x40
#define CANINT_ERRI 0x20
#define CANINT_TX2I 0x10
#define CANINT_TX1I 0x08
#define CANINT_TX0I 0x04
#define CANINT_RX1I 0x02
#define CANINT_RX0I 0x01

#define RXF0SIDH 0x00
#define RXF0SIDL 0x01
#define RXF0EID8 0x02
#define RXF0EID0 0x03

#define RXF1SIDH 0x04
#define RXF1SIDL 0x05
#define RXF1EID8 0x06
#define RXF1EID0 0x07

#define RXF2SIDH 0x08
#define RXF2SIDL 0x09
#define RXF2EID8 0x0A
#define RXF2EID0 0x0B

#define RXF3SIDH 0x10
#define RXF3SIDL 0x11
#define RXF3EID8 0x12
#define RXF3EID0 0x13

#define RXF4SIDH 0x14
#define RXF4SIDL 0x15
#define RXF4EID8 0x16
#define RXF4EID0 0x17

#define RXF5SIDH 0x18
#define RXF5SIDL 0x19
#define RXF5EID8 0x1A
#define RXF5EID0 0x1B

#define RXM0SIDH 0x20
#define RXM0SIDL 0x21
#define RXM0EID8 0x22
#define RXM0EID0 0x23

#define RXM1SIDH 0x24
#define RXM1SIDL 0x25
#define RXM1EID8 0x26
#define RXM1EID0 0x27

#define TXB0CTRL 0x30
#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
#define TXB0EID8 0x33
#define TXB0EID0 0x34
#define TXB0DLC  0x35
#define TXB0D0   0x36
#define TXB0D1   0x37
#define TXB0D2   0x38
#define TXB0D3   0x39
#define TXB0D4   0x3A
#define TXB0D5   0x3B
#define TXB0D6   0x3C
#define TXB0D7   0x3D

#define TXB1CTRL 0x40
#define TXB1SIDH 0x41
#define TXB1SIDL 0x42
#define TXB1EID8 0x43
#define TXB1EID0 0x44
#define TXB1DLC  0x45
#define TXB1D0   0x46
#define TXB1D1   0x47
#define TXB1D2   0x48
#define TXB1D3   0x49
#define TXB1D4   0x4A
#define TXB1D5   0x4B
#define TXB1D6   0x4C
#define TXB1D7   0x4D

#define TXB2CTRL 0x50
#define TXB2SIDH 0x51
#define TXB2SIDL 0x52
#define TXB2EID8 0x53
#define TXB2EID0 0x54
#define TXB2DLC  0x55
#define TXB2D0   0x56
#define TXB2D1   0x57
#define TXB2D2   0x58
#define TXB2D3   0x59
#define TXB2D4   0x5A
#define TXB2D5   0x5B
#define TXB2D6   0x5C
#define TXB2D7   0x5D

#define TXBCTRL_ABTF 0x40
#define TXBCTRL_MLOA 0x20
#define TXBCTRL_TXERR 0x10
#define TXBCTRL_TXREQ 0x08
#define TXBCTRL_TXP1 0x02
#define TXBCTRL_TXP0 0x01

#define TXBSIDL_EXIDE 0x08
#define TXBDLC_RTR 0x40

#define RXB0CTRL 0x60
#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0EID8 0x63
#define RXB0EID0 0x64
#define RXB0DLC  0x65
#define RXB0D0   0x66
#define RXB0D1   0x67
#define RXB0D2   0x68
#define RXB0D3   0x69
#define RXB0D4   0x6A
#define RXB0D5   0x6B
#define RXB0D6   0x6C
#define RXB0D7   0x6D

#define RXB1CTRL 0x70
#define RXB1SIDH 0x71
#define RXB1SIDL 0x72
#define RXB1EID8 0x73
#define RXB1EID0 0x74
#define RXB1DLC  0x75
#define RXB1D0   0x76
#define RXB1D1   0x77
#define RXB1D2   0x78
#define RXB1D3   0x79
#define RXB1D4   0x7A
#define RXB1D5   0x7B
#define RXB1D6   0x7C
#define RXB1D7   0x7D

#define RXBCTRL_RXM1 0x40
#define RXBCTRL_RXM0 0x20
#define RXBCTRL_RXRTR 0x08
#define RXB0CTRL_BUKT 0x04
#define RXB0CTRL_FILHIT 0x01
#define RXB1CTRL_FILHIT2 0x04
#define RXB1CTRL_FILHIT1 0x02
#define RXB1CTRL_FILHIT0 0x01

#define RXBSIDL_SRR 0x10
#define RXBSIDL_IDE 0x08

#define RXBDLC_RTR 0x40

#define SPI_RXB0SIDH 0x00
#define SPI_RXB0D0   0x02
#define SPI_RXB1SIDH 0x04
#define SPI_RXB1D0   0x06

#define SPI_TXB0SIDH 0x00
#define SPI_TXB0D0   0x01
#define SPI_TXB1SIDH 0x02
#define SPI_TXB1D0   0x03
#define SPI_TXB2SIDH 0x04
#define SPI_TXB2D0   0x05

#define SPI_RTS_TXB0 0x01
#define SPI_RTS_TXB1 0x02
#define SPI_RTS_TXB2 0x04

#ifndef F_MCP
  #ifdef F_CPU
    #define F_MCP F_CPU
  #else
    #define F_MCP 16000000
  #endif
#endif

typedef struct {
  uint8_t SIDH;
  uint8_t SIDL;
  uint8_t EID8;
  uint8_t EID0;
  uint8_t DLC;
  uint8_t data[8];
} can_message;

#define RX_BUFFER_COUNT 4
#define TX_BUFFER_COUNT 4

uint8_t mcp2515_read(uint8_t address);
void mcp2515_multi_read(uint8_t address, uint8_t* buf, uint8_t length);
