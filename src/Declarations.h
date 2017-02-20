//==================================================
// Copyright (C) 2015 Team 1538 / The Holy Cows
//==================================================

#ifndef __DECLARATIONS_H__
#define __DECLARATIONS_H__

#define ROBOT_HZ						400

#define COWCONSTANTS_DEFAULT_FILE		"/home/lvuser/constants.ini"

// Number of accumulation periods to be summed
#define COWGYRO_RING_SIZE				5
// Length of an accumulation period in seconds
#define COWGYRO_ACCUMULATION_PERIOD		1.0

#define GYRO_SENSITIVITY				0.007

// CAN IDs
#define CAN_ID_WINCH					1
#define CAN_ID_CONVEYOR_BOTTOM			2
#define CAN_ID_CONVEYOR_TOP				3
#define CAN_ID_BALL_INTAKE				4
#define CAN_ID_ARM						5
#define CAN_ID_TURRET					6
#define CAN_ID_SHOOTER_R				7
#define CAN_ID_SHOOTER_L				8
#define CAN_ID_SHOOTER_FEEDER			9
#define CAN_ID_GEAR_INTAKE				10
#define CAN_ID_DRIVE_L_A				11
#define CAN_ID_DRIVE_L_B				12
#define CAN_ID_DRIVE_L_C				13
#define CAN_ID_DRIVE_R_A				14
#define CAN_ID_DRIVE_R_B				15
#define CAN_ID_DRIVE_R_C				16

#define MXP_QEI_1_A						10
#define MXP_QEI_1_B						11
#define MXP_QEI_2_A						12
#define MXP_QEI_2_B						13
#define MXP_QEI_3_A						18
#define MXP_QEI_3_B						19
#define MXP_QEI_4_A						20
#define MXP_QEI_4_B						21
#define MXP_QEI_5_A						22
#define MXP_QEI_5_B						23

// Analog inputs
#define ANALOG_GYRO						0

// Digital outputs	
#define RELAY_COMPRESSOR				1

// Digital inputs
#define DIGITAL_PRESSURE_SWITCH			1

#define SOLENOID_PTO					7
#define SOLENOID_ARM					6
#define SOLENOID_LOCK					5
#define SOLENOID_UNLOCK					4

#endif
