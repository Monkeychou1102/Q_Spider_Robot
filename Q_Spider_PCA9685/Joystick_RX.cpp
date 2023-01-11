#include "Joystick_RX.h"

// JoyStick - Buttons
unsigned int X_Axis = 0, Y_Axis = 0;
bool Button_UP = 0, Button_DOWN = 0, Button_LEFT = 0, Button_RIGHT = 0;
bool Button_SELECT = 0, Button_START = 0;
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
    if (Button_Status & 0x0400)
    {
        Button_UP = 0;
    }
    else
    {
        Button_UP = 1;
    }

    // DOWN
    if (Button_Status & 0x1000)
    {
        Button_DOWN = 0;
    }
    else
    {
        Button_DOWN = 1;
    }

    // LEFT
    if (Button_Status & 0x2000)
    {
        Button_LEFT = 0;
    }
    else
    {
        Button_LEFT = 1;
    }

    // RIGHT
    if (Button_Status & 0x0800)
    {
        Button_RIGHT = 0;
    }
    else
    {
        Button_RIGHT = 1;
    }

    // SELECT
    if (Button_Status & 0x8000)
    {
        Button_SELECT = 0;
    }
    else
    {
        Button_SELECT = 1;
    }

    // START
    if (Button_Status & 0x4000)
    {
        Button_START = 0;
    }
    else
    {
        Button_START = 1;
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

bool Read_Button_UP(void)
{
    return Button_UP;
}

bool Read_Button_DOWN(void)
{
    return Button_DOWN;
}

bool Read_Button_LEFT(void)
{
    return Button_LEFT;
}

bool Read_Button_RIGHT(void)
{
    return Button_RIGHT;
}

bool Read_Button_SELECT(void)
{
    return Button_SELECT;
}

bool Read_Button_START(void)
{
    return Button_START;
}

//---------------------------------------------------
void Joystick_Clear_Button_Status(void)
{
    // Clear Button Status
    Button_UP = 0;
    Button_DOWN = 0;
    Button_LEFT = 0;
    Button_RIGHT = 0;
    Button_SELECT = 0;
    Button_START = 0;
    Button_Status = 0;
}
