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

Shooter::Shooter(uint32_t motorA, uint32_t motorB, uint32_t motorFeeder)
:
	m_Speed(0),
	m_PIDEnabled(false)
{
	m_MotorA = new CANTalon(motorA);

	m_MotorA->ConfigEncoderCodesPerRev(360);

	m_MotorA->SetControlMode(CANTalon::kSpeed);
	m_MotorA->SetPID(CONSTANT("SHOOTER_P"), CONSTANT("SHOOTER_I"), CONSTANT("SHOOTER_D"), CONSTANT("SHOOTER_F"));
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
	if(abs(m_MotorA->GetSpeed() - m_MotorA->GetSetpoint()) < CONSTANT("SHOOTER_TOLERANCE"))
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

double Shooter::GetMotorASpeed()
{
	//return m_EncoderASpeed;
	return 0;
}


void Shooter::ResetConstants()
{
//	m_PID_A_Rate->UpdateConstants(CONSTANT("SHOOTER_A_P"), CONSTANT("SHOOTER_A_I"), CONSTANT("SHOOTER_A_D"), 0);
//
//	m_LPF_A->UpdateBeta(CONSTANT("SHOOTER_A_BETA"));
	m_MotorA->SetPID(CONSTANT("SHOOTER_P"), CONSTANT("SHOOTER_I"), CONSTANT("SHOOTER_D"), CONSTANT("SHOOTER_F"));

	m_MotorA->ConfigNominalOutputVoltage(CONSTANT("SHOOTER_NOM_A_F"), CONSTANT("SHOOTER_NOM_A_R"));
	m_MotorA->ConfigPeakOutputVoltage(CONSTANT("SHOOTER_PEAK_A_F"), CONSTANT("SHOOTER_PEAK_A_R"));

	m_MotorB->ConfigNominalOutputVoltage(CONSTANT("SHOOTER_NOM_B_F"), CONSTANT("SHOOTER_NOM_B_R"));
	m_MotorB->ConfigPeakOutputVoltage(CONSTANT("SHOOTER_PEAK_B_F"), CONSTANT("SHOOTER_PEAK_B_R"));


}

void Shooter::Handle()
{
	//(rate/360)rps

//	double lpfValue = m_LPF_A->Calculate(m_EncoderASpeed);
//
//	double motorAPID_Rate = m_PID_A_Rate->Calculate(lpfValue);
//
//	double motorAPID_Voltage = m_PID_A_Rate->GetSetpoint() * (CONSTANT("SHOOTER_MAX_VLT") / robotVoltage) * CONSTANT("SHOOTER_A_F");
//
//
//	if(m_MotorA)
//	{
//		if(m_PIDEnabled)
//		{
//			//std::cout << "S1: " << m_EncoderASpeed << "VA: " << motorAPID_Voltage << " RV: " << robotVoltage << std::endl;
//
//			m_MotorA->Set(motorAPID_Rate + motorAPID_Voltage);
//		}
//		else
//		{
//			m_MotorA->Set(m_Speed);
//		}
//	}

	//SmartDashboard::PutNumber("Shooter RPM raw", -m_EncoderASpeed);
	//SmartDashboard::PutNumber("Shooter RPM smooth", -lpfValue);

	//m_MotorA->Set(m_Speed);

	if (!m_PIDEnabled)
	{
		m_MotorA->Set(0);
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
