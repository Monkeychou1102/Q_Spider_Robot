#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TimerOne.h>
#include "NRF24L01_RX.h"
#include "Spider_Movement_Control.h"

//-----------------------------------------------------------------------------------------------------
#define Control_with_Remote_Controller true

/* The following definitions are for debug mode */
#define Debug_Calibration_Servo_by_UART false
#define Debug_Remote_RX_Mode false
#define Debug_Movement_Performenace_Self_Test false

#define Start_Delay_ms 2000

//------------------------------------------------------------------
uint8_t index = 0;
uint8_t buffer_Read[4] = {0};
char chr = 0;
char Flag_Update = 0;
char Moving_State = 0;

//------------------------- Timer1 Interrupt------------------------
#define Freq_10kHz 100 // 100us -> 10kHz
// Time counter and event for Timer1 ISR
unsigned int Tick_01ms = 0, Tick_1ms = 0, Tick_10ms = 0, Tick_100ms = 0, Tick_1000ms = 0;
unsigned char Event_1ms = 0, Event_10ms = 0, Event_100ms = 0, Event_1000ms = 0;
// Tick and Counter
unsigned int Timer1_Tick = 0;
unsigned int Timer1_Tick_Max = 1000; // 1000*0.1ms = 100ms

//---------------------------------------------------------------------
// JoyStick
unsigned int X_Axis = 0, Y_Axis = 0;
unsigned int Button_UP = 0, Button_DOWN = 0, Button_LEFT = 0, Button_RIGHT = 0;
unsigned int Button_SELECT = 0, Button_START = 0;
unsigned int Button_Status = 0;

//---------------------------------------------------------------------
void setup()
{
    Serial.begin(9600);

#if Debug_Calibration_Servo_by_UART == true
    Serial.println("*************************************************************************************");
    Serial.println("< Notice >");
    Serial.println("1. Baud Rate is 9600");
    Serial.println("2. ARDUINO SERIAL MONITOR setting must be NL(newline), otherwise process will fail.");
    Serial.println("*************************************************************************************");
    Serial.println(" ");
    Serial.println("Input State for Test");
#endif

    PCA9689_Servo_PWM_Control_Init();

#if Debug_Remote_RX_Mode == true
    Front_Left(F_L_Leg_Min, F_L_Foot_90);
    Front_Right(F_R_Leg_Min, F_R_Foot_90);
    Back_Left(B_L_Leg_Min, B_L_Foot_90);
    Back_Right(B_R_Leg_Min, B_R_Foot_90);

    while (1)
    {
        ;
    }
#endif

#if Control_with_Remote_Controller == true
    // Timer 1 - Interrupt
    Timer1.initialize(Freq_10kHz);      // 100us -> 10kHz
    Timer1.attachInterrupt(Timer1_ISR); // Define ISR of Timer1

    // SPI
    NRF24L01_Arduino_Pin_Define();

    SPI.begin();
    delay(Dummy_Delay_ms);
    NRF24L01_init_io();
    NRF24L01_Init_RX_Mode();
#endif

    Spider_Standby();
    delay(Start_Delay_ms);
}

//---------------------------------------------------------------------
void loop()
{
#if Debug_Calibration_Servo_by_UART == true

    if (Serial.available() > 0)
    {
        Serial.println("Starting Process...");
        Serial.print("Input Data = ");

        // Notice the following statement...
        // The setting in ARDUINO SERIAL MONITOR must be NL(newline), otherwise no '\n' will be received.
        while ((chr = Serial.read()) != '\n')
        {
            if (chr >= '0' && chr <= '9')
            {
                if (index < 1)
                {
                    buffer_Read[index] = chr - 48;
                    Serial.print(buffer_Read[index]);
                }
                index++;
            }
        }
        Serial.println(" ");
        Serial.print("State = ");
        Serial.println(buffer_Read[index - 1]);

        Spider_Forward(buffer_Read[index - 1], 100);

        index = 0;
        Serial.println("********************END PROCESS***********************");
        Serial.println(" ");
    }
#endif

#if Debug_Movement_Performenace_Self_Test == true
    // Forward
    Spider_Forward(Moving_State, Servo_Delay_ms); // For Test without remote control command
    Moving_State++;
    if (Moving_State > 6)
        Moving_State = 1;
#endif

#if Control_with_Remote_Controller == true

    if (Event_1000ms)
    {
        NRF24L01_Receive_Data();
        Handle_Receive_Data(RX_Buffer);

        Report_Joystick_Status();

        Event_1000ms = 0;
    }

#endif
}

//------------------------------------------------------
//                    Timer1 - ISR
//              (Timer1 Interrupt -> 0.1ms )
//------------------------------------------------------
void Timer1_ISR()
{
    Tick_01ms++;             // Minimum Tick = 0.1 ms
    if (Tick_01ms % 10 == 0) // 0.1 ms*10 = 1 ms
    {
        // Event - 1ms
        Event_1ms = 1;
        Tick_1ms = 0;
    }

    if (Tick_01ms % 100 == 0) // 0.1 ms*100 = 10 ms
    {
        // Event - 10ms
        Event_10ms = 1;
        Tick_10ms = 0;
    }

    if (Tick_01ms % 1000 == 0) // 0.1 ms*1000 = 100 ms
    {
        // Event - 100ms
        Event_100ms = 1;
        Tick_100ms = 0;
    }

    if (Tick_01ms % 10000 == 0) // 0.1 ms*10000 = 1000 sec
    {
        // Event - 1000ms
        Event_1000ms = 1;
        Tick_1000ms = 0;
    }
}

//-------------------------------------------------------------------------
//                            Handle Receive Data
//-------------------------------------------------------------------------
void Handle_Receive_Data(unsigned char *Buffer)
{
    unsigned char Wireless_Key1 = 0, Wireless_Key2 = 0;
    unsigned char Wireless_Key1_Num = 0x55; // Key1
    unsigned char Wireless_Key2_Num = 0x55; // Key2

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

    Serial.println("enter...");

    if ((Wireless_Key1 != Wireless_Key1_Num) || (Wireless_Key2 != Wireless_Key2_Num))
    {
        Serial.println(Wireless_Key1);
        Serial.println(Wireless_Key2);
        Serial.println("fail...return!!");
        return;
    }

    Serial.println("pass...");

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
// Read Joystick AD Status and Report to PC by UART
void Report_Joystick_Status(void)
{
    unsigned int X = 0;

    Serial.print("X:");
    Serial.print(X_Axis);
    Serial.print(" ");

    Serial.print("Y:");
    Serial.print(Y_Axis);
    Serial.print(" ");

    Serial.print("UP:");
    Serial.print(Button_UP);
    Serial.print(" ");

    Serial.print("DOWN:");
    Serial.print(Button_DOWN);
    Serial.print(" ");

    Serial.print("LEFT:");
    Serial.print(Button_LEFT);
    Serial.print(" ");

    Serial.print("RIGHT:");
    Serial.print(Button_RIGHT);
    Serial.print(" ");

    Serial.print("SELECT:");
    Serial.print(Button_SELECT);
    Serial.print(" ");

    Serial.print("START:");
    Serial.print(Button_START);
    Serial.print(" ");

    Serial.println();
}
