/*
 * Shooter.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: kchau
 */

#include <Subsystems/Shooter.h>
#include <WPILib.h>
#include "../CowConstants.h"
#include <cmath>
#include "../CowPixyBlock.h"

Shooter::Shooter(uint32_t motorA, uint32_t motorB, uint32_t motorFeeder)
:
	m_Speed(0),
	m_PIDEnabled(false)
{
	m_MotorA = new CANTalon(motorA);

	m_MotorA->ConfigEncoderCodesPerRev(360);

	m_MotorA->SetControlMode(CANTalon::kSpeed);
	m_MotorA->SetP(CONSTANT("SHOOTER_P"));
	m_MotorA->SetI(CONSTANT("SHOOTER_I"));
	m_MotorA->SetD(CONSTANT("SHOOTER_D"));
	m_MotorA->SetF(CONSTANT("SHOOTER_F"));
	m_MotorA->SetSetpoint(0);


	m_MotorB = new CANTalon(motorB);

	m_MotorB->SetControlMode(CANTalon::kFollower);
	m_MotorB->Set(motorA);
	m_MotorB->SetClosedLoopOutputDirection(true);

	m_MotorFeeder = new CANTalon(motorFeeder);
	m_MotorFeeder->Set(0);

	//m_MotorA->ConfigPeakOutputVoltage(0,-12);
	//m_MotorB->ConfigPeakOutputVoltage(12,0);

	m_MotorA->ConfigNominalOutputVoltage(CONSTANT("SHOOTER_NOM_A_F"), CONSTANT("SHOOTER_NOM_A_R"));
	m_MotorA->ConfigPeakOutputVoltage(CONSTANT("SHOOTER_PEAK_A_F"), CONSTANT("SHOOTER_PEAK_A_R"));

	m_MotorB->ConfigNominalOutputVoltage(CONSTANT("SHOOTER_NOM_B_F"), CONSTANT("SHOOTER_NOM_B_R"));
	m_MotorB->ConfigPeakOutputVoltage(CONSTANT("SHOOTER_PEAK_B_F"), CONSTANT("SHOOTER_PEAK_B_R"));

	m_MotorA->DisableNominalClosedLoopVoltage();
	m_MotorA->SetNominalClosedLoopVoltage(CONSTANT("SHOOTER_MAX_VLT"));

	m_MotorB->DisableNominalClosedLoopVoltage();
	m_MotorB->SetNominalClosedLoopVoltage(CONSTANT("SHOOTER_MAX_VLT"));

	m_Lpf = new CowLib::CowLPF(CONSTANT("SPEED_LPF"));
}

void Shooter::SetManualSpeed(float speed)
{
	m_Speed = -speed;
}

void Shooter::SetAutoSpeed(float speedA)
{
	m_MotorA->Set(-speedA);
}

void Shooter::SetPIDState(bool state)
{
	m_PIDEnabled = state;

	if (m_PIDEnabled)
	{
		m_MotorA->SetControlMode(CANTalon::kSpeed);
	}
	else
	{
		m_MotorA->SetControlMode(CANTalon::kPercentVbus);
	}
}

void Shooter::SetFeederSpeed(float speed)
{
	m_MotorFeeder->Set(speed);
}

bool Shooter::IsOnTarget()
{
	bool targeted = false;
//	if(m_PID_A_Rate->OnTarget(CONSTANT("SHOOTER_A_TOLERANCE")))
//	{
//		targeted = true;
//	}
	if(fabs(-m_MotorA->GetSpeed() - m_MotorA->GetSetpoint()) < CONSTANT("SHOOTER_TOLERANCE"))
	{
		targeted = true;
	}
	return targeted;
}

bool Shooter::HasShotBall()
{
	bool hasShot = false;
//	if(m_PIDEnabled &&
//			m_PID_A_Rate->GetSetpoint() != 0 )
//	{
//		if(fabs(m_PID_A_Rate->GetError()) > 500)
//		{
//			hasShot = true;
//		}
//	}
	return hasShot;

}

double Shooter::GetSpeed()
{
	return -m_MotorA->GetSpeed();
}


void Shooter::ResetConstants()
{
//	m_PID_A_Rate->UpdateConstants(CONSTANT("SHOOTER_A_P"), CONSTANT("SHOOTER_A_I"), CONSTANT("SHOOTER_A_D"), 0);
//
//	m_LPF_A->UpdateBeta(CONSTANT("SHOOTER_A_BETA"));
	m_MotorA->SetP(CONSTANT("SHOOTER_P"));
	m_MotorA->SetI(CONSTANT("SHOOTER_I"));
	m_MotorA->SetD(CONSTANT("SHOOTER_D"));
	m_MotorA->SetF(CONSTANT("SHOOTER_F"));

	m_MotorA->ConfigNominalOutputVoltage(CONSTANT("SHOOTER_NOM_A_F"), CONSTANT("SHOOTER_NOM_A_R"));
	m_MotorA->ConfigPeakOutputVoltage(CONSTANT("SHOOTER_PEAK_A_F"), CONSTANT("SHOOTER_PEAK_A_R"));

	m_MotorB->ConfigNominalOutputVoltage(CONSTANT("SHOOTER_NOM_B_F"), CONSTANT("SHOOTER_NOM_B_R"));
	m_MotorB->ConfigPeakOutputVoltage(CONSTANT("SHOOTER_PEAK_B_F"), CONSTANT("SHOOTER_PEAK_B_R"));

	m_MotorA->DisableNominalClosedLoopVoltage();
	m_MotorA->SetNominalClosedLoopVoltage(CONSTANT("SHOOTER_MAX_VLT"));

	m_MotorB->DisableNominalClosedLoopVoltage();
	m_MotorB->SetNominalClosedLoopVoltage(CONSTANT("SHOOTER_MAX_VLT"));

	m_Lpf->UpdateBeta(CONSTANT("SPEED_LPF"));
}

void Shooter::Handle()
{

	if(m_PIDEnabled)
	{
		//Adjust the shooter speed based on distance
		PixyBlock *pixy = PixyBlock::GetInstance();
		pixy->SetRead(true);

		double y_pos = m_Lpf->Calculate(pixy->GetY());
		std::cout << "y pos:" << y_pos << std::endl;

		//double shooterSpeed = CONSTANT("SHOOTER_SLOPE") * y_pos + CONSTANT("SHOOTER_B");
		//SetAutoSpeed(shooterSpeed);
		if((y_pos >= CONSTANT("SHOOTER_HEIGHT_RANGE_1")) && (y_pos < CONSTANT("SHOOTER_HEIGHT_RANGE_2")))
		{
			SetAutoSpeed(CONSTANT("SHOOTER_HEIGHT_SPEED_1"));
		}
		else if((y_pos >= CONSTANT("SHOOTER_HEIGHT_RANGE_2")) && (y_pos < CONSTANT("SHOOTER_HEIGHT_RANGE_3")))
		{
			SetAutoSpeed(CONSTANT("SHOOTER_HEIGHT_SPEED_2"));
		}
		else if((y_pos >= CONSTANT("SHOOTER_HEIGHT_RANGE_3")) && (y_pos < CONSTANT("SHOOTER_HEIGHT_RANGE_4")))
		{
			SetAutoSpeed(CONSTANT("SHOOTER_HEIGHT_SPEED_3"));
		}
		else if((y_pos >= CONSTANT("SHOOTER_HEIGHT_RANGE_4")) && (y_pos < CONSTANT("SHOOTER_HEIGHT_RANGE_5")))
		{
			SetAutoSpeed(CONSTANT("SHOOTER_HEIGHT_SPEED_4"));
		}
		else if((y_pos >= CONSTANT("SHOOTER_HEIGHT_RANGE_5")) && (y_pos < CONSTANT("SHOOTER_HEIGHT_RANGE_6")))
		{
			SetAutoSpeed(CONSTANT("SHOOTER_HEIGHT_SPEED_5"));
		}
		else if((y_pos >= CONSTANT("SHOOTER_HEIGHT_RANGE_6")) && (y_pos < CONSTANT("SHOOTER_HEIGHT_RANGE_7")))
		{
			SetAutoSpeed(CONSTANT("SHOOTER_HEIGHT_SPEED_6"));
		}
		else
		{
			SetAutoSpeed(CONSTANT("SHOOTER_HEIGHT_SPEED_4"));
		}
	}

	if (!m_PIDEnabled)
	{
		m_MotorA->Set(0);
		PixyBlock *pixy = PixyBlock::GetInstance();
		pixy->SetRead(false);
	}
	SmartDashboard::PutNumber("MotorA Current", m_MotorA->GetOutputCurrent());
	SmartDashboard::PutNumber("MotorB Current", m_MotorB->GetOutputCurrent());
	SmartDashboard::PutNumber("Shooter speed", -m_MotorA->GetSpeed());
}

void Shooter::Reset()
{
	m_PIDEnabled = false;
	m_Speed = 0;
	//m_EncoderASpeed = 0;
}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}
