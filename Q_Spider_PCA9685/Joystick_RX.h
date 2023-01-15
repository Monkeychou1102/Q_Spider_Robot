#include <Arduino.h>

#define Verified_Key1 0x55 // Key1
#define Verified_Key2 0x55 // Key2

void Joystick_Receive_Data(unsigned char *Buffer);
uint16_t Read_X_Axis(void);
uint16_t Read_Y_Axis(void);
bool Read_Button_Up(void);
bool Read_Button_Down(void);
bool Read_Button_Left(void);
bool Read_Button_Right(void);
bool Read_Button_Select(void);
bool Read_Button_Start(void);
void Joystick_Clear_Button_Status(void);
