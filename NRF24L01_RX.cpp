#include "NRF24L01_RX.h"
#include <SPI.h>
#include <TimerOne.h>

//---------------------------------------------------------------
//                    Define Timer and Counter
//---------------------------------------------------------------
unsigned int Tick_01ms = 0, Tick_1ms = 0, Tick_10ms = 0, Tick_100ms = 0, Tick_1000ms = 0;
unsigned char Event_1ms = 0, Event_10ms = 0, Event_100ms = 0, Event_1000ms = 0;
//---------------------------------------------------------------------------------------
unsigned char TX_ADDRESS[TX_ADR_WIDTH] = 
{
  0x34, 0x43, 0x10, 0x10, 0x01
};
unsigned char RX_Buffer[TX_PLOAD_WIDTH] = {0};      // RX Buffer
unsigned char TX_Buffer[TX_PLOAD_WIDTH] = {0};      // TX Buffer



// Receive
void Receive_Data(void)
{
  unsigned char Status_Data = SPI_Read(STATUS);      // Read STATUS
  
  if(Status_Data & RX_DR)        // Condition?
  {
    SPI_Read_Buf(RD_RX_PLOAD, RX_Buffer, TX_PLOAD_WIDTH);
    SPI_RW_Reg(FLUSH_RX, 0);
  }
  
  SPI_RW_Reg(WRITE_REG + STATUS, Status_Data);
  delay(40);
}

//------------------------------------------------------------------
void init_io(void)
{
//  digitalWrite(IRQ, 0);
  digitalWrite(CE, 0);         // Chip Enable
  digitalWrite(CSN, 1);        // SPI STOP -> Disable
}

//------------------------------------------------------------------
// Send a byte data to SPI TX Register
unsigned char SPI_RW(unsigned char Input_Data)
{
//  return SPI.transfer(Input_Data);
}

//-------------------------------------------------------------------
// Write data to NRF24L01
unsigned char SPI_RW_Reg(unsigned char Register, unsigned char Input_Data)
{
  unsigned char Response = 0;
  
  digitalWrite(CSN, 0);     // CSN -> Low, Start SPI
  SPI_RW(Register);         // Select NRF24L01 Register
  SPI_RW(Input_Data);       // Transmit a byte data to NRF24L01
  digitalWrite(CSN, 1);     // CSN -> High, Stop SPI
  
  return Response;
}

//---------------------------------------------------------------------
unsigned char SPI_Read(unsigned char Register)
{
  unsigned char Register_Data = 0;
  
  digitalWrite(CSN, 0);     // CSN -> Low, Start SPI
  SPI_RW(Register);         // Select NRF24L01 Register
  Register_Data = SPI_RW(0);       // Read Register Data
  digitalWrite(CSN, 1);     // CSN -> High, Stop SPI
  
  return Register_Data;
}

//-------------------------------------------------------------------
unsigned char SPI_Read_Buf(unsigned char Register, unsigned char *Buffer, unsigned char WIDTH)
{
  unsigned char Status = 0, i = 0;
  
  digitalWrite(CSN, 0);     // CSN -> Low, Start SPI
  Status = SPI_RW(Register);
  
  for(i = 0; i < WIDTH; i++)
  {
    Buffer[i] = SPI_RW(0);      // Read Register Data
  }
  digitalWrite(CSN, 1);     // CSN -> High, Stop SPI
  
  return Status;
}

//--------------------------------------------------------------------------
unsigned char SPI_Write_Buf(unsigned char Register, unsigned char *Buffer, unsigned char WIDTH)
{
  unsigned char Status = 0, i = 0;
  
  digitalWrite(CSN, 0);     // CSN -> Low, Start SPI
  Status = SPI_RW(Register);
  
  for(i = 0; i < WIDTH; i++)
  {
    SPI_RW(*Buffer++);      // Write data to Buffer 
  }
  digitalWrite(CSN, 1);     // CSN -> High, Stop SPI
  
  return Status;
}

//----------------------------------------------------------------------------
void RX_Mode(void)
{
  digitalWrite(CE, 0);
  
  SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  
  
  SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);
  SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  
  SPI_RW_Reg(WRITE_REG + RF_CH, 40);
  SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  
  
  SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07); 
  
  SPI_RW_Reg(WRITE_REG + CONFIG, 0x0F);
  
  digitalWrite(CE, 1);  
}
