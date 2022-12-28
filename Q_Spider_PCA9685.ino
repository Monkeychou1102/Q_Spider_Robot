#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "NRF24L01_RX.h"

//-----------------------------------------------------------------------------------------------------
//#define Debug_Mode
//----------------------------------------------------------------------------------------------------
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!

//-----------------------------------------------------------------------------------------------------
#define SERVOMIN 150  // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600  // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

//-----------------------------------------------------------------
// Define Servos on Spider Robot
#define F_Left_Leg 4
#define F_Left_Foot 5
#define F_Right_Leg 7
#define F_Right_Foot 6
#define B_Left_Leg 3
#define B_Left_Foot 2
#define B_Right_Leg 0
#define B_Right_Foot 1

//-------------Define Limited operating range for servos---------------
//-----------------Foot Angle and PWM Command Definition---------------
// Max and Min
#define F_L_Leg_Max 500
#define F_L_Leg_Min 1700
#define F_R_Leg_Max 2400
#define F_R_Leg_Min 1100
#define B_L_Leg_Max 1000
#define B_L_Leg_Min 2100
#define B_R_Leg_Max 1700
#define B_R_Leg_Min 600
// Leg - 45 dgree
#define F_L_Leg_n45 1500
#define F_R_Leg_n45 1400
#define B_L_Leg_n45 1900
#define B_R_Leg_n45 700
// Leg - 0 dgree
#define F_L_Leg_0 1150
#define F_R_Leg_0 1700
#define B_L_Leg_0 1500
#define B_R_Leg_0 1000
// Leg - 45 dgree
#define F_L_Leg_45 900
#define F_R_Leg_45 2000
#define B_L_Leg_45 1200
#define B_R_Leg_45 1300
//-----------------Foot Angle and PWM Command Definition---------------
// Foot - 90 dgree
#define F_L_Foot_90 550
#define F_R_Foot_90 2200
#define B_L_Foot_90 2300
#define B_R_Foot_90 400
// Foot - 135 dgree
#define F_L_Foot_135 1000
#define F_R_Foot_135 1800
#define B_L_Foot_135 1900
#define B_R_Foot_135 750
// Foot - 180 dgree
#define F_L_Foot_180 1500
#define F_R_Foot_180 1200
#define B_L_Foot_180 1300
#define B_R_Foot_180 1300
//------------------------------------------------------------------
// our servo # counter
#define servonum_Max 15 // Max. number of servo in use.
uint8_t index = 0;
uint8_t buffer_Read[4] = {0};
uint8_t servonum = 0;
uint16_t microsec = 0;
char chr = 0;
char Flag_Update = 0;
char Moving_State = 0;

//---------------------------------------------------------------------
void PCA9685_Control(char servo_num, unsigned int pulse_width_us);
void Front_Left(unsigned int leg_position, unsigned foot_position);
void Front_Right(unsigned int leg_position, unsigned foot_position);
void Back_Left(unsigned int leg_position, unsigned foot_position);
void Back_Right(unsigned int leg_position, unsigned foot_position);
void Spider_Standby(void);
void Spider_Release(void);

//---------------------------------------------------------------------
void setup()
{
#ifdef Debug_Mode
    Serial.begin(9600);

    Serial.println("*************************************************************************************");
    Serial.println("< Notice >");
    Serial.println("1. Baud Rate is 9600");
    Serial.println("2. ARDUINO SERIAL MONITOR setting must be NL(newline), otherwise process will fail.");
    Serial.println("*************************************************************************************");
    Serial.println(" ");
    Serial.println("Input State for Test");
#endif

    pwm.begin();

    pwm.setOscillatorFrequency(25500000);
    pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates
    delay(100);
    /*
    Front_Left(F_L_Leg_Min, F_L_Foot_90);
    Front_Right(F_R_Leg_Min, F_R_Foot_90);
    Back_Left(B_L_Leg_Min, B_L_Foot_90);
    Back_Right(B_R_Leg_Min, B_R_Foot_90);
    while(1);
*/
    Spider_Standby();
    delay(2000);
}

//---------------------------------------------------------------------
void loop()
{
#ifdef Debug_Mode
    // Debug Mode
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
#else
    // Normal Mode
    // Forward
    Spider_Forward(Moving_State, 250);
    Moving_State++;
    if (Moving_State > 6)
        Moving_State = 1;
#endif
}

//--------------------------------------------------------------------
void Spider_Release(void)
{
    Front_Left(0, 0);
    Front_Right(0, 0);
    Back_Left(0, 0);
    Back_Right(0, 0);
}
//--------------------------------------------------------------------
void Spider_Standby(void)
{
    Front_Left(F_L_Leg_45, F_L_Foot_90);
    Front_Right(F_R_Leg_45, F_R_Foot_90);
    Back_Left(B_L_Leg_n45, B_L_Foot_90);
    Back_Right(B_R_Leg_n45, B_R_Foot_90);
}

//----------------------------------------------------------------------
void Spider_Forward(char state, unsigned int delay_time)
{
    if (state == 0)
    {
        Spider_Standby();
        delay(delay_time);
    }
    else if (state == 1)
    {
        Front_Right(F_R_Leg_0, F_R_Foot_90);
        Back_Right(B_R_Leg_n45, B_R_Foot_135);
        delay(delay_time);
        Back_Right(B_R_Leg_0, B_R_Foot_135);
        delay(delay_time);
        Back_Right(B_R_Leg_0, B_R_Foot_90);
        delay(delay_time);
    }
    else if (state == 2)
    {
        Front_Right(F_R_Leg_0, F_R_Foot_135);
        delay(delay_time);
        Front_Right(F_R_Leg_Max, F_R_Foot_135);
        delay(delay_time);
        Front_Right(F_R_Leg_Max, F_R_Foot_90);
    }
    else if (state == 3)
    {
        Front_Left(F_L_Leg_0, F_L_Foot_90);
        Front_Right(F_R_Leg_45, F_R_Foot_90);
        Back_Left((B_L_Leg_Min - 200), B_L_Foot_90);
        Back_Right((B_R_Leg_n45 - 200), B_R_Foot_90);
        delay(delay_time*2);
    }
    else if (state == 4)
    {
        Back_Left(B_L_Leg_Min, B_L_Foot_135);
        delay(delay_time);
        Back_Left(B_L_Leg_0, B_L_Foot_135);
        delay(delay_time);
        Back_Left(B_L_Leg_0, B_L_Foot_90);
        delay(delay_time);
    }
    else if (state == 5)
    {
        Front_Left(F_L_Leg_0, F_L_Foot_135);
        delay(delay_time);
        Front_Left(F_L_Leg_Max, F_L_Foot_135);
        delay(delay_time);
        Front_Left(F_L_Leg_Max, F_L_Foot_90);
        delay(delay_time);
    }
    else if (state == 6)
    {
        Front_Left(F_L_Leg_45, F_L_Foot_90);
        Front_Right(F_R_Leg_0, F_R_Foot_90);
        Back_Left(B_L_Leg_n45, B_L_Foot_90);
        Back_Right(B_R_Leg_Min, B_R_Foot_90);
        delay(delay_time*2);
    }
    else if (state == 9)
    {
        Spider_Release();
    }
    else
    {
        Spider_Standby();
    }
}

//--------------------------------------------------------------------
void Front_Left(unsigned int leg_position, unsigned foot_position)
{
    PCA9685_Control(F_Left_Leg, leg_position);
    PCA9685_Control(F_Left_Foot, foot_position);
}

//--------------------------------------------------------------------
void Front_Right(unsigned int leg_position, unsigned foot_position)
{
    PCA9685_Control(F_Right_Leg, leg_position);
    PCA9685_Control(F_Right_Foot, foot_position);
}

//--------------------------------------------------------------------
void Back_Left(unsigned int leg_position, unsigned foot_position)
{
    PCA9685_Control(B_Left_Leg, leg_position);
    PCA9685_Control(B_Left_Foot, foot_position);
}

//--------------------------------------------------------------------
void Back_Right(unsigned int leg_position, unsigned foot_position)
{
    PCA9685_Control(B_Right_Leg, leg_position);
    PCA9685_Control(B_Right_Foot, foot_position);
}

//--------------------------------------------------------------------
void PCA9685_Control(char servo_num, unsigned int pulse_width_us)
{
    pwm.writeMicroseconds(servo_num, pulse_width_us);
}
