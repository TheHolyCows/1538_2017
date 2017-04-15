/*
 * Turret.cpp
 *
 *  Created on: Jan 29, 2017
 *      Author: hchau
 */

#include <CowLib/CowPixy.h>
#include <Subsystems/Turret.h>
#include "../CowConstants.h"
#include <CANTalon.h>

Turret::Turret(uint8_t motor)
:
	m_Motor(0),
	m_Speed(0),
	m_PositionMax(CONSTANT("TURRET_POSITION_MAX")),
	m_PositionMin(CONSTANT("TURRET_POSITION_MIN")),
	m_SetPoint(0),
	m_OperatorOverride(false),
	m_PixyCenterX(CONSTANT("PIXY_CENTER_X")),
	m_PixyScaleFactor(CONSTANT("PIXY_SCALE_FACTOR"))
{
	m_Motor = new CANTalon(motor);
	m_Motor->SetControlMode(CANTalon::kPosition);
	m_Motor->SetPID(CONSTANT("TURRET_P"), CONSTANT("TURRET_I"), CONSTANT("TURRET_D"), 0);
	m_Motor->SetSetpoint(0);
	m_Lpf = new CowLib::CowLPF(CONSTANT("PIXY_LPF"));
	m_PixySolenoid = new Solenoid(3);
	m_LightTime = 0;

}

Turret::~Turret() {

}

void Turret::SetAutoTurret(bool turret)
{
	m_AutoTurret = turret;
}

void Turret::Handle()
{
	PixyBlock *pixy = PixyBlock::GetInstance();
	double position;

	// Suppose turret is at position 40000, need to move -40000 left
	// packet.x is going to retu	rn a value around 239
	// center is 159.5
	// position = (159.5 - 239) * 500 = -39750
	// turret at -40000, packet.x is at 79, ( 159.5 - 79) * 500 = ...

	if(m_AutoTurret && m_LightTime == 0)
	{
		m_LightTime = Timer::GetFPGATimestamp();
	}

	if(m_AutoTurret)
	{
		m_PixySolenoid->Set(true);

		double x_pos = m_Lpf->Calculate(pixy->GetX());

		position = (CONSTANT("PIXY_CENTER_X") - x_pos);
		if((Timer::GetFPGATimestamp() - m_LightTime) > 1)
		{
			SetSetPoint(m_SetPoint + (position * CONSTANT("PIXY_SCALE_FACTOR")));
			m_Motor->SetSetpoint(m_SetPoint);
		}
		else
		{
			SetSetPoint(0);
			m_Motor->SetSetpoint(m_SetPoint);
			m_Lpf->UpdateBeta(CONSTANT("PIXY_LPF"));
		}
	}
	else
	{
		m_LightTime = 0;
		SetSetPoint(0);
		m_Motor->SetSetpoint(m_SetPoint);
		m_PixySolenoid->Set(false);
		m_Lpf->UpdateBeta(CONSTANT("PIXY_LPF"));
	}
}

void Turret::SetSetPoint(float sp)
{
	if (sp > m_PositionMax)
	{
		m_SetPoint = m_PositionMax;
	}
	else if(sp < m_PositionMin)
	{
		m_SetPoint = m_PositionMin;
	}
	else
	{
		m_SetPoint = sp;
	}
}

float Turret::GetSetPoint()
{
	return m_SetPoint;
}

bool Turret::IsOnTarget()
{
	if(fabs(m_LastPixyValue - CONSTANT("PIXY_CENTER_X") < 0.2) && m_AutoTurret && ((Timer::GetFPGATimestamp() - m_LightTime) > 1))
	{
		return true;
	}
	else
	{
		return false;
	}

}

void Turret::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Turret::ResetConstants()
{
	m_Motor->SetPID(CONSTANT("TURRET_P"), CONSTANT("TURRET_I"), CONSTANT("TURRET_D"), 0);
	m_Lpf->UpdateBeta(CONSTANT("PIXY_LPF"));
}

void Turret::SetTurretOperatorOverride(bool override)
{
	m_OperatorOverride = override;
}
