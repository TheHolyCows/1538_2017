//==================================================
// Copyright (C) 2017 Team 1538 / The Holy Cows
//==================================================

#ifndef __COW_ROBOT_H__
#define __COW_ROBOT_H__

#include <CowLib/CowLogger.h>
#include <WPILib.h>
#include <CANTalon.h>
#include <CowLib/CowPixy.h>
#include <Subsystems/Conveyer.h>
#include <Subsystems/Light.h>
#include "Controllers/GenericController.h"
#include "CowLib/CowLib.h"
#include "CowControlBoard.h"
#include "CounterBase.h"
#include "CowConstants.h"
#include "CowLib/CowAlphaNum.h"
#include "CowGyro.h"
#include "Subsystems/Shooter.h"
#include "Subsystems/Conveyer.h"
#include "Subsystems/Turret.h"
#include "Subsystems/GearIntake.h"

class CowRobot
{
public:
private:
	int m_DSUpdateCount;
	
	GenericController *m_Controller;
	
	// Drive Motors
	CANTalon *m_LeftDriveA;
	CANTalon *m_LeftDriveB;
	CANTalon *m_LeftDriveC;

	CANTalon *m_RightDriveA;
	CANTalon *m_RightDriveB;
	CANTalon *m_RightDriveC;
	CANTalon *m_Climber;
	
	CowLib::CowGyro *m_Gyro;
	Encoder *m_DriveEncoder;
	Encoder *m_DriveEncoderLeft;
	Encoder *m_DriveEncoderRight;

	Encoder *m_ShooterEncoder;

	Shooter *m_Shooter;
	Conveyer *m_ConveyerUpper;
	Conveyer *m_ConveyerLower;
	Conveyer *m_BallIntakeConveyer;
	Turret *m_Turret;

	Solenoid *m_LightSolenoid;
	Light *m_Light;

	GearIntake *m_GearIntake;

	PowerDistributionPanel *m_PowerDistributionPanel;

	CowLib::CowAlphaNum *m_LEDDisplay;
	CowLib::CowLogger *m_WebServer;

	float m_LeftDriveValue;
	float m_RightDriveValue;

	double m_PreviousGyroError;
	double m_PreviousDriveError;

	double m_MatchTime;
	double m_StartTime;
	float m_AutoOffsetAngle;

	void SetLeftMotors(float val);
	void SetRightMotors(float val);

public:
	CowRobot();
	void Reset();
	void GyroHandleCalibration();
	void GyroFinalizeCalibration();
	void SetController(GenericController *controller);
	void PrintToDS();
	double GetDriveDistance();
	bool DriveDistance(double distance);
	bool DriveDistanceWithHeading(double heading, double distance, double speed);
	bool DriveWithHeading(double heading, double speed);
	bool DriveWithHeading(double heading, double speed, double maxSpeed);
	void DriveSpeedTurn(float speed, float turn, bool quickTurn);
	void DriveLeftRight(float leftDriveValue, float rightDriveValue);
	bool TurnToHeading(double heading);
	void QuickTurn(float turn);
	
	void StartTime();

	CowLib::CowAlphaNum *GetDisplay()
	{
		return m_LEDDisplay;
	}

	PowerDistributionPanel *GetPowerDistributionPanel()
	{
		return m_PowerDistributionPanel;
	}

	CowLib::CowGyro *GetGyro()
	{
		return CowLib::CowGyro::GetInstance();
	}

	CANTalon *GetClimber()
	{
		return m_Climber;
	}

	Encoder *GetEncoder()
	{
		return m_DriveEncoder;
	}

	Shooter *GetShooter()
	{
		return m_Shooter;
	}

	Conveyer *GetConveyerUpper()
	{
		return m_ConveyerUpper;
	}

	Conveyer *GetConveyerLower()
	{
		return m_ConveyerLower;
	}

	Conveyer *GetBallIntakeConveyer()
	{
		return m_BallIntakeConveyer;
	}

	Turret *GetTurret()
	{
		return m_Turret;
	}

	GearIntake *GetGearIntake()
	{
		return m_GearIntake;
	}

	Pixy *GetPixy()
	{
		return Pixy::GetInstance();
	}

	Light *GetLight()
	{
		return m_Light;
	}

	void UseLeftEncoder()
	{
		m_DriveEncoder = m_DriveEncoderLeft;
	}

	void UseRightEncoder()
	{
		m_DriveEncoder = m_DriveEncoderRight;
	}

	void ResetEncoders()
	{
		m_DriveEncoderLeft->Reset();
		m_DriveEncoderRight->Reset();
	}

	void AddAutoOffsetAngle()
	{
		m_AutoOffsetAngle += 2.5;
		std::stringstream tempSS;
		tempSS << "Offset Angle: " << m_AutoOffsetAngle << std::endl;
		DriverStation::GetInstance().ReportError(tempSS.str());
	}

	void DecAutoOffsetAngle()
	{
		m_AutoOffsetAngle -= 2.5;
		std::stringstream tempSS;
		tempSS << "Offset Angle: " << m_AutoOffsetAngle << std::endl;
		DriverStation::GetInstance().ReportError(tempSS.str());
	}

	float GetAutoOffsetAngle()
	{
		return m_AutoOffsetAngle;
	}

	void handle();
};

#endif
