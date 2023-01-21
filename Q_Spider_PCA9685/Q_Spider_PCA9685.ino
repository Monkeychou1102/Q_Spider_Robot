#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TimerOne.h>
#include "NRF24L01_RX.h"
#include "Spider_Movement_Control.h"
#include "Joystick_RX.h"

//**************************************************************************
//           Select Operation Mode (Debug Mode or Normal Mode)
//**************************************************************************
// Normal Mode
#define Control_with_Remote_Controller true
#define Enable_Serial_To_Monitor_Joystick_State_With_Remote_Controller false

// The following definitions are for debug mode
#define Debug_Calibration_Servo_by_UART false
#define Debug_Remote_RX_Mode false
#define Debug_Movement_Performenace_Self_Test false
//**************************************************************************

#define Start_Delay_ms 1000
uint8_t index = 0;
uint8_t buffer_Read[4] = {0};
char chr = 0;
char Flag_Update = 0;
char Moving_State = 0;

//------------------------- Timer1 Interrupt------------------------
#define Freq_10kHz 100 // 100us -> 10kHz
// Time counter and event for Timer1 ISR
unsigned int Tick_01ms = 0;
unsigned char Event_1ms = 0, Event_10ms = 0, Event_100ms = 0, Event_1000ms = 0;
// Tick and Counter
unsigned int Timer1_Tick = 0;
unsigned int Timer1_Tick_Max = 1000; // 1000*0.1ms = 100ms

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
    Joystick_Clear_Button_Status();
#endif
    Spider_Posture_Low(Servo_Delay_ms);
    delay(Start_Delay_ms);
    Spider_Standby(Servo_Delay_ms);
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

        Spider_Forward(buffer_Read[index - 1], Servo_Delay_ms);

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

    if (Event_100ms)
    {
        NRF24L01_Receive_Data();
        Joystick_Receive_Data(RX_Buffer);

#if Enable_Serial_To_Monitor_Joystick_State_With_Remote_Controller == true
        Joystick_Report_Status();
#endif

        Handle_Joystick_Command();

        Event_100ms = 0;
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
        Event_1ms = 1;
    }

    if (Tick_01ms % 100 == 0) // 0.1 ms*100 = 10 ms
    {
        Event_10ms = 1;
    }

    if (Tick_01ms % 1000 == 0) // 0.1 ms*1000 = 100 ms
    {
        Event_100ms = 1;
    }

    if (Tick_01ms % 10000 == 0) // 0.1 ms*10000 = 1000 sec
    {
        Event_1000ms = 1;
    }
}

//---------------------------------------------------------------------------
// Read Joystick AD Status and Report to PC by UART
void Joystick_Report_Status(void)
{
    Serial.print("X:");
    Serial.print(Read_X_Axis());
    Serial.print(" ");

    Serial.print("Y:");
    Serial.print(Read_Y_Axis());
    Serial.print(" ");

    Serial.print("UP:");
    Serial.print(Read_Button_Up());
    Serial.print(" ");

    Serial.print("DOWN:");
    Serial.print(Read_Button_Down());
    Serial.print(" ");

    Serial.print("LEFT:");
    Serial.print(Read_Button_Left());
    Serial.print(" ");

    Serial.print("RIGHT:");
    Serial.print(Read_Button_Right());
    Serial.print(" ");

    Serial.print("SELECT:");
    Serial.print(Read_Button_Select());
    Serial.print(" ");

    Serial.print("START:");
    Serial.print(Read_Button_Start());
    Serial.print(" ");

    Serial.println();
}

//----------------------------------------------------------------------------
// Read Joystick Status
void Handle_Joystick_Command(void)
{
    // For this mode, only allow one command can be excuted at the same time.
    if (Read_Button_Up()) // UP Command
    {
        Spider_Forward(Moving_State, Servo_Delay_ms);
    }
    else if (Read_Button_Down()) // DOWN Command
    {
        Spider_Backward(Moving_State, Servo_Delay_ms);
    }
    else if (Read_Button_Left()) // LEFT Command
    {
        Spider_Turn_Left(Moving_State, Servo_Delay_ms);
    }
    else if (Read_Button_Right()) // RIGHT Command
    {
        Spider_Turn_Right(Moving_State, Servo_Delay_ms);
    }
    else if (Read_Button_Select()) // SELECT
    {
        Spider_Posture_Low(Servo_Delay_ms);
    }
    else if (Read_Button_Start()) // START
    {
        Spider_Standby(Servo_Delay_ms);
    }
    else
    {
        // If No Button is Pressed, need to retun, do nothing.
        return;
    }

    Moving_State++;
    if (Moving_State > 6)
    {
        Moving_State = 1;
    }
    Joystick_Clear_Button_Status(); // For avoiding the wrong instruction when the communication lose
}
