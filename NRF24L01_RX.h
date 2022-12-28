
//--------------------------------------------------------
//             Define NRF24L01 Command
//--------------------------------------------------------
#define  READ_REG          0x00       // Read Register Command
#define  WRITE_REG         0x20       // Write Register Command
#define  RD_RX_PLOAD       0x61       // Read RX Payload
#define  WR_TX_PLOAD       0xA0       // Write TX Payload
#define  FLUSH_TX          0xE1       // Transmit FIFO Command
#define  FLUSH_RX          0xE2       // Receive FIFO Command
#define  REUSE_TX_PLOAD    0xE3       // Re-load TX Payload
#define  NOP               0xFF       // Reserved

//--------------------------------------------------------
#define  RX_DR            0x40        
#define  TX_DS            0x20
#define  MAX_RT           0x10

//-----------------------------------------------------------------------
//            Define NRF24L01 Register Address(Refer to spec.)
//-----------------------------------------------------------------------
#define  CONFIG           0x00       // Configuration Register
#define  EN_AA            0x01       // Enable Auto Acknowledgment Function
#define  EN_RXADDR        0x02       // Enable RX Addresses
#define  SETUP_AW         0x03       // Setup of Address Widths
#define  SETUP_RETR       0x04       // Setup of Automatic Retransmission
#define  RF_CH            0x05       // RF Channel
#define  RF_SETUP         0x06       // RF Setup Register
#define  STATUS           0x07       // Status Register
#define  OBSERVE_TX       0x08       // Transmit Observer Register
#define  CD               0x09       // CD
#define  RX_ADDR_P0       0x0A       // Receive Address Data Pipe 0
#define  RX_ADDR_P1       0x0B       // Receive Address Data Pipe 1
#define  RX_ADDR_P2       0x0C       // Receive Address Data Pipe 2
#define  RX_ADDR_P3       0x0D       // Receive Address Data Pipe 3
#define  RX_ADDR_P4       0x0E       // Receive Address Data Pipe 4
#define  RX_ADDR_P5       0x0F       // Receive Address Data Pipe 5
#define  TX_ADDR          0x10       // Transmit Address(PTX device used only and LSByte is written first)
#define  RX_PW_P0         0x11       // Pipe 0 RX Width - Number of Bytes in RX Payload
#define  RX_PW_P1         0x12       // Pipe 1 RX Width - Number of Bytes in RX Payload
#define  RX_PW_P2         0x13       // Pipe 2 RX Width - Number of Bytes in RX Payload
#define  RX_PW_P3         0x14       // Pipe 3 RX Width - Number of Bytes in RX Payload
#define  RX_PW_P4         0x15       // Pipe 4 RX Width - Number of Bytes in RX Payload
#define  RX_PW_P5         0x16       // Pipe 5 RX Width - Number of Bytes in RX Payload
#define  FIFO_STATUS      0x17       // FIFO Status Register
//#define  DYNPD            0x1C

//------------------------------------------------------------------------
//                   Define Arduino Pin for NRF24L01
//------------------------------------------------------------------------
// UNO
#define  CE      9
#define  CSN     10
//#define  IRQ     10

//-----------------------------------------------------------------------
#define  TX_ADR_WIDTH      5         // 5 bytes length for TX Address 
#define  TX_PLOAD_WIDTH    32        // 32 bytes length for TX Buffer

//--------------------------------------------------------------------
void Receive_Data(void);
void init_io(void);
unsigned char SPI_RW(unsigned char Input_Data);
unsigned char SPI_RW_Reg(unsigned char Register, unsigned char Input_Data);
unsigned char SPI_Read(unsigned char Register);
unsigned char SPI_Read_Buf(unsigned char Register, unsigned char *Buffer, unsigned char WIDTH);
unsigned char SPI_Write_Buf(unsigned char Register, unsigned char *Buffer, unsigned char WIDTH);
void RX_Mode(void);








