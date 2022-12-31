#include <Arduino.h>
#include "Spider_Movement_Control.h"
#include <Adafruit_PWMServoDriver.h>

uint8_t servonum = 0;

//----------------------------------------------------------------------------------------------------
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!

//-------------------------------------------------------------------
void PCA9689_Servo_PWM_Control_Init(void)
{
    pwm.begin();

    pwm.setOscillatorFrequency(25500000);
    pwm.setPWMFreq(SERVO_FREQ); // Analog servos run at ~50 Hz updates
    delay(Dummy_Delay_ms);
}

//--------------------------------------------------------------------
void PCA9685_Control(char servo_num, unsigned int pulse_width_us)
{
    pwm.writeMicroseconds(servo_num, pulse_width_us);
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
        delay(delay_time * 2);
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
        delay(delay_time * 2);
    }
    else
    {
        Spider_Standby();
    }
}

//----------------------------------------------------------------------
void Spider_Backward(char state, unsigned int delay_time)
{
    if (state == 0)
    {
        Spider_Standby();
        delay(delay_time);
    }
    else if (state == 1)
    {
        Back_Left(F_R_Leg_0, F_R_Foot_90);
        Front_Left(B_R_Leg_n45, B_R_Foot_135);
        delay(delay_time);
        Front_Left(B_R_Leg_0, B_R_Foot_135);
        delay(delay_time);
        Front_Left(B_R_Leg_0, B_R_Foot_90);
        delay(delay_time);
    }
    else if (state == 2)
    {
        Back_Left(F_R_Leg_0, F_R_Foot_135);
        delay(delay_time);
        Back_Left(F_R_Leg_Max, F_R_Foot_135);
        delay(delay_time);
        Back_Left(F_R_Leg_Max, F_R_Foot_90);
    }
    else if (state == 3)
    {
        Back_Right(F_L_Leg_0, F_L_Foot_90);
        Back_Left(F_R_Leg_45, F_R_Foot_90);
        Front_Right((B_L_Leg_Min - 200), B_L_Foot_90);
        Front_Left((B_R_Leg_n45 - 200), B_R_Foot_90);
        delay(delay_time * 2);
    }
    else if (state == 4)
    {
        Front_Right(B_L_Leg_Min, B_L_Foot_135);
        delay(delay_time);
        Front_Right(B_L_Leg_0, B_L_Foot_135);
        delay(delay_time);
        Front_Right(B_L_Leg_0, B_L_Foot_90);
        delay(delay_time);
    }
    else if (state == 5)
    {
        Back_Right(F_L_Leg_0, F_L_Foot_135);
        delay(delay_time);
        Back_Right(F_L_Leg_Max, F_L_Foot_135);
        delay(delay_time);
        Back_Right(F_L_Leg_Max, F_L_Foot_90);
        delay(delay_time);
    }
    else if (state == 6)
    {
        Back_Right(F_L_Leg_45, F_L_Foot_90);
        Back_Left(F_R_Leg_0, F_R_Foot_90);
        Front_Right(B_L_Leg_n45, B_L_Foot_90);
        Front_Left(B_R_Leg_Min, B_R_Foot_90);
        delay(delay_time * 2);
    }
    else
    {
        Spider_Standby();
    }
}
