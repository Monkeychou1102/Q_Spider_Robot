#include "NRF24L01_RX.h"
#include <SPI.h>
#include <TimerOne.h>

//-----------------------------------------------------------------------
unsigned char TX_ADDRESS[TX_ADR_WIDTH] =
    {
        0x34, 0x43, 0x10, 0x10, 0x01};

unsigned char RX_Buffer[TX_PLOAD_WIDTH] = {0}; // RX Buffer
unsigned char TX_Buffer[TX_PLOAD_WIDTH] = {0}; // TX Buffer

//-----------------------------------------------------------------------
void NRF24L01_Receive_Data(void)
{
  unsigned char Status_Data = NRF24L01_SPI_Byte_Read(STATUS); // Read STATUS

  if (Status_Data & RX_DR) // Condition?
  {
    NRF24L01_SPI_Read_Buf(RD_RX_PLOAD, RX_Buffer, TX_PLOAD_WIDTH);
    NRF24L01_Write_Byte_To_Register(FLUSH_RX, 0);
  }

  NRF24L01_Write_Byte_To_Register(WRITE_REG + STATUS, Status_Data);
}

//------------------------------------------------------------------
void NRF24L01_init_io(void)
{
  //  digitalWrite(IRQ, 0);
  NRF24L01_Chip_Select();
  NRF24L01_Stop_SPI_Function();
}

//------------------------------------------------------------------
// Send a byte data to SPI TX Register
unsigned char NRF24L01_SPI_RW(unsigned char Input_Data)
{
  return SPI.transfer(Input_Data);
}

//-------------------------------------------------------------------
// Write data to NRF24L01
unsigned char NRF24L01_Write_Byte_To_Register(unsigned char Register, unsigned char Input_Data)
{
  unsigned char Response = 0;

  NRF24L01_Start_SPI_Function();

  NRF24L01_SPI_RW(Register);   // Select NRF24L01 Register
  NRF24L01_SPI_RW(Input_Data); // Transmit a byte data to NRF24L01

  NRF24L01_Stop_SPI_Function();

  return Response;
}

//---------------------------------------------------------------------
unsigned char NRF24L01_SPI_Byte_Read(unsigned char Register)
{
  unsigned char Register_Data = 0;

  NRF24L01_Start_SPI_Function();

  NRF24L01_SPI_RW(Register);          // Select NRF24L01 Register
  Register_Data = NRF24L01_SPI_RW(0); // Read Register Data

  NRF24L01_Stop_SPI_Function();

  return Register_Data;
}

//-------------------------------------------------------------------
unsigned char NRF24L01_SPI_Read_Buf(unsigned char Register, unsigned char *Buffer, unsigned char WIDTH)
{
  unsigned char Status = 0, i = 0;

  NRF24L01_Start_SPI_Function();

  Status = NRF24L01_SPI_RW(Register);

  for (i = 0; i < WIDTH; i++)
  {
    Buffer[i] = NRF24L01_SPI_RW(0); // Read Register Data
  }

  NRF24L01_Stop_SPI_Function();

  return Status;
}

//--------------------------------------------------------------------------
unsigned char NRF24L01_SPI_Write_Buf(unsigned char Register, unsigned char *Buffer, unsigned char WIDTH)
{
  unsigned char Status = 0, i = 0;

  NRF24L01_Start_SPI_Function();

  Status = NRF24L01_SPI_RW(Register);

  for (i = 0; i < WIDTH; i++)
  {
    NRF24L01_SPI_RW(*Buffer++); // Write data to Buffer
  }

  NRF24L01_Stop_SPI_Function();

  return Status;
}

//----------------------------------------------------------------------------
void NRF24L01_Init_RX_Mode(void)
{
  NRF24L01_Chip_Select();

  NRF24L01_SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);

  NRF24L01_Write_Byte_To_Register(WRITE_REG + EN_AA, 0x01);
  NRF24L01_Write_Byte_To_Register(WRITE_REG + EN_RXADDR, 0x01);
  NRF24L01_Write_Byte_To_Register(WRITE_REG + RF_CH, 40);
  NRF24L01_Write_Byte_To_Register(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);

  NRF24L01_Write_Byte_To_Register(WRITE_REG + RF_SETUP, 0x07);

  NRF24L01_Write_Byte_To_Register(WRITE_REG + CONFIG, 0x0F);

  NRF24L01_Not_Chip_Select();
}

//--------------------------------------------------------------------------
void NRF24L01_Start_SPI_Function(void)
{
  digitalWrite(CSN_Pin, Start_SPI);
}

//--------------------------------------------------------------------------
void NRF24L01_Stop_SPI_Function(void)
{
  digitalWrite(CSN_Pin, Stop_SPI);
}

//--------------------------------------------------------------------------
void NRF24L01_Chip_Select(void)
{
  digitalWrite(CE_Pin, Set_Chip_Enable);
}

//--------------------------------------------------------------------------
void NRF24L01_Not_Chip_Select(void)
{
  digitalWrite(CE_Pin, Set_Chip_Disable);
}

//---------------------------------------------------------------------------
void NRF24L01_Arduino_Pin_Define(void)
{
  pinMode(CE_Pin, OUTPUT);
  pinMode(CSN_Pin, OUTPUT);
}
