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
	m_SetPoint(0)
{
	m_Motor = new CANTalon(motor);
	m_Motor->SetControlMode(CANTalon::kPosition);
	m_Motor->SetPID(CONSTANT("TURRET_P"), CONSTANT("TURRET_I"), CONSTANT("TURRET_D"), 0);
	m_Motor->SetSetpoint(0);
}

Turret::~Turret() {

}

void Turret::Handle()
{
	double position;
	//Pixy::PixyPacket packet = Pixy::GetInstance()->GetPacket();

//	m_Speed = m_PID->Calculate(packet.x);
//
//	if(m_PID->OnTarget(CONSTANT("TURRET_TOLERANCE")))
//	{
//		m_PID->ResetIntegrator();
//	}

	//m_Motor->Set(m_Speed);

	//m_Motor->Set

	position = m_Motor->GetPosition();

	std::cout<<"Position: "<<position<<std::endl;

	m_Motor->SetSetpoint(m_SetPoint);
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

void Turret::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Turret::ResetConstants()
{
	m_Motor->SetPID(CONSTANT("TURRET_P"), CONSTANT("TURRET_I"), CONSTANT("TURRET_D"), 0);
}
