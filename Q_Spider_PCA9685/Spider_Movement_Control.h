//-----------------------------------------------------------------------------------------------------
#define SERVOMIN 150  // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600  // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 600     // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400    // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define Dummy_Delay_ms 100
#define Servo_Delay_ms 50

// Define Servo locations on Spider Robot
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
#define Leg_Offset 100
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
#define B_R_Leg_n45 500
// Leg - 0 dgree
#define F_L_Leg_0 1150
#define F_R_Leg_0 1700
#define B_L_Leg_0 1500
#define B_R_Leg_0 (1000 + Leg_Offset)
// Leg - 45 dgree
#define F_L_Leg_45 800
#define F_R_Leg_45 2000
#define B_L_Leg_45 1200
#define B_R_Leg_45 1300

//-----------------Foot Angle and PWM Command Definition---------------
#define Foot_Offset 200
// Foot - 90 dgree
#define F_L_Foot_90 (550 + Foot_Offset)  // Fine tune to improve moving performance
#define F_R_Foot_90 (2200 - Foot_Offset) // Fine tune to improve moving performance
#define B_L_Foot_90 (2300 - Foot_Offset) // Fine tune to improve moving performance
#define B_R_Foot_90 (400 + Foot_Offset)  // Fine tune to improve moving performance
// Foot - 135 dgree
#define F_L_Foot_135 (1000 + Foot_Offset)
#define F_R_Foot_135 (1800 - Foot_Offset)
#define B_L_Foot_135 (1900 - Foot_Offset)
#define B_R_Foot_135 (750 + Foot_Offset)
// Foot - 180 dgree
#define F_L_Foot_180 1500
#define F_R_Foot_180 1200
#define B_L_Foot_180 1300
#define B_R_Foot_180 1300

// our servo # counter
#define servonum_Max 15 // Max. number of servo in use.

void PCA9689_Servo_PWM_Control_Init(void);
void PCA9685_Control(char servo_num, unsigned int pulse_width_us);
void Front_Left(unsigned int leg_position, unsigned foot_position);
void Front_Right(unsigned int leg_position, unsigned foot_position);
void Back_Left(unsigned int leg_position, unsigned foot_position);
void Back_Right(unsigned int leg_position, unsigned foot_position);
void Spider_Standby(void);
void Spider_Release(void);
void Spider_Forward(char state, unsigned int delay_time);
void Spider_Backward(char state, unsigned int delay_time);
