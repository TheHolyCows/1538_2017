/*
 * Shooter.cpp
 *
 *  Created on: Feb 17, 2016
 *      Author: kchau
 */

#include <Subsystems/Shooter.h>
#include "../CowConstants.h"
#include <cmath>

Shooter::Shooter(uint8_t motorA, Encoder* encoderA)
:
	m_MotorA(0),
	m_Speed(0),
	m_EncoderA(encoderA),
	m_PIDEnabled(false),
	m_EncoderASpeed(0)
{
	m_MotorA = new Talon(motorA);
	m_PID_A_Rate = new CowLib::CowPID(CONSTANT("SHOOTER_A_P"), CONSTANT("SHOOTER_A_I"), CONSTANT("SHOOTER_A_D"), CONSTANT("SHOOTER_A_F"));

	m_LPF_A = new CowLib::CowLPF(CONSTANT("SHOOTER_A_BETA"));
}

void Shooter::SetManualSpeed(float speed)
{
	m_Speed = speed;
}

void Shooter::SetAutoSpeed(float speedA)
{
	m_PID_A_Rate->SetSetpoint(-speedA);
}

void Shooter::SetPIDState(bool state)
{
	m_PIDEnabled = state;
}

bool Shooter::IsOnTarget()
{
	bool targeted = false;
	if(m_PID_A_Rate->OnTarget(CONSTANT("SHOOTER_A_TOLERANCE")))
	{
		targeted = true;
	}
	return targeted;
}

bool Shooter::HasShotBall()
{
	bool hasShot = false;
	if(m_PIDEnabled &&
			m_PID_A_Rate->GetSetpoint() != 0 )
	{
		if(fabs(m_PID_A_Rate->GetError()) > 500)
		{
			hasShot = true;
		}
	}
	return hasShot;

}

double Shooter::GetMotorASpeed()
{
	return m_EncoderASpeed;
}


void Shooter::ResetConstants()
{
	m_PID_A_Rate->UpdateConstants(CONSTANT("SHOOTER_A_P"), CONSTANT("SHOOTER_A_I"), CONSTANT("SHOOTER_A_D"), 0);

	m_LPF_A->UpdateBeta(CONSTANT("SHOOTER_A_BETA"));
}

void Shooter::Handle()
{
	//(rate/360)rps
	m_EncoderASpeed = (m_EncoderA->GetRate() / 360.0) * 60.0;

	double robotVoltage = DriverStation::GetInstance().GetBatteryVoltage();

	double lpfValue = m_LPF_A->Calculate(m_EncoderASpeed);

	double motorAPID_Rate = m_PID_A_Rate->Calculate(lpfValue);

	double motorAPID_Voltage = m_PID_A_Rate->GetSetpoint() * (CONSTANT("SHOOTER_MAX_VLT") / robotVoltage) * CONSTANT("SHOOTER_A_F");


	if(m_MotorA)
	{
		if(m_PIDEnabled)
		{
			std::cout << "S1: " << m_EncoderASpeed << "VA: " << motorAPID_Voltage << " RV: " << robotVoltage << std::endl;

			m_MotorA->Set(motorAPID_Rate + motorAPID_Voltage);
		}
		else
		{
			m_MotorA->Set(m_Speed);
		}
	}

	if(m_Dashboard)
	{
		m_Dashboard->PutNumber("Shooter RPM raw", -m_EncoderASpeed);
		m_Dashboard->PutNumber("Shooter RPM smooth", -lpfValue);
	}
}

void Shooter::Reset()
{
	m_PIDEnabled = false;
	m_Speed = 0;
	m_EncoderASpeed = 0;
}

Shooter::~Shooter() {
	// TODO Auto-generated destructor stub
}

void Shooter::SetDashboard(SmartDashboard* dashboard)
{
	m_Dashboard = dashboard;
}
