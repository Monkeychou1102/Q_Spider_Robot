#include <Arduino.h>

#define Verified_Key1 0x55 // Key1
#define Verified_Key2 0x55 // Key2

void Joystick_Receive_Data(unsigned char *Buffer);
uint16_t Read_X_Axis(void);
uint16_t Read_Y_Axis(void);
bool Read_Button_UP(void);
bool Read_Button_DOWN(void);
bool Read_Button_LEFT(void);
bool Read_Button_RIGHT(void);
bool Read_Button_SELECT(void);
bool Read_Button_START(void);
void Joystick_Clear_Button_Status(void);
