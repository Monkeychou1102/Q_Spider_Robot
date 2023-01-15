#include "Joystick_RX.h"

// Define buttons on joystick
#define Check_Button_A_State 0x0400
#define Check_Button_B_State 0x0800
#define Check_Button_C_State 0x1000
#define Check_Button_D_State 0x2000
#define Check_Button_E_State 0x4000
#define Check_Button_F_State 0x8000

// JoyStick - Buttons
unsigned int X_Axis = 0, Y_Axis = 0;
bool Button_Up = 0, Button_Down = 0, Button_Left = 0, Button_Right = 0;
bool Button_Select = 0, Button_Start = 0;
unsigned int Button_Status = 0;

//-------------------------------------------------------------------------
//                            Handle Receive Data
//-------------------------------------------------------------------------
void Joystick_Receive_Data(unsigned char *Buffer)
{
    unsigned char Wireless_Key1 = 0, Wireless_Key2 = 0;

    // Define the data format for JoyStick
    // Byte_0 + Byte_1<<8 = X Axis
    // Byte_2 + Byte_3<<8 = Y Axis
    //             Byte_4 = Status of Buttons includes SELECT, START, UP, DOWN, LEFT and RIGHT.
    //       Bit:    F           E           D           C           B           A           9           8           7 ~ 0
    //             SELECT      START        LEFT        DOWN       RIGHT         UP         TBD         TBD           TBD

    X_Axis = *(Buffer + 0) + *(Buffer + 1) << 8;
    Y_Axis = *(Buffer + 2) + *(Buffer + 3) << 8;
    Button_Status = *(Buffer + 4) << 8;

    Wireless_Key1 = *(Buffer + 5);
    Wireless_Key2 = *(Buffer + 6);

    if ((Wireless_Key1 != Verified_Key1) || (Wireless_Key2 != Verified_Key2))
    {
        return;
    }

    // UP
    if (Button_Status & Check_Button_D_State)
    {
        Button_Up = 0;
    }
    else
    {
        Button_Up = 1;
    }

    // DOWN
    if (Button_Status & Check_Button_B_State)
    {
        Button_Down = 0;
    }
    else
    {
        Button_Down = 1;
    }

    // LEFT
    if (Button_Status & Check_Button_C_State)
    {
        Button_Left = 0;
    }
    else
    {
        Button_Left = 1;
    }

    // RIGHT
    if (Button_Status & Check_Button_A_State)
    {
        Button_Right = 0;
    }
    else
    {
        Button_Right = 1;
    }

    // SELECT
    if (Button_Status & Check_Button_F_State)
    {
        Button_Select = 0;
    }
    else
    {
        Button_Select = 1;
    }

    // START
    if (Button_Status & Check_Button_E_State)
    {
        Button_Start = 0;
    }
    else
    {
        Button_Start = 1;
    }

    //--------------------Reset Wireless Key Status---------------------------
    // Clear Wireless_Key1 and Wireless_Key2
    *(Buffer + 5) = 0; // Wireless_Key1
    *(Buffer + 6) = 0; // Wireless_Key2
}

//---------------------------------------------------------------------------
// Return joystick's states
uint16_t Read_X_Axis(void)
{
    return X_Axis;
}

uint16_t Read_Y_Axis(void)
{
    return Y_Axis;
}

bool Read_Button_Up(void)
{
    return Button_Up;
}

bool Read_Button_Down(void)
{
    return Button_Down;
}

bool Read_Button_Left(void)
{
    return Button_Left;
}

bool Read_Button_Right(void)
{
    return Button_Right;
}

bool Read_Button_Select(void)
{
    return Button_Select;
}

bool Read_Button_Start(void)
{
    return Button_Start;
}

//---------------------------------------------------
void Joystick_Clear_Button_Status(void)
{
    // Clear Button Status
    Button_Up = 0;
    Button_Down = 0;
    Button_Left = 0;
    Button_Right = 0;
    Button_Select = 0;
    Button_Start = 0;
    Button_Status = 0;
}
