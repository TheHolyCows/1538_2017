/*
 * Turret.cpp
 *
 *  Created on: Jan 29, 2017
 *      Author: hchau
 */

#include <CowLib/CowPixy.h>
#include <Subsystems/Turret.h>
#include "../CowConstants.h"

Turret::Turret(uint8_t motor)
:
	m_Motor(0),
	m_Speed(0)
{
	m_Motor = new Talon(motor);
	m_PID = new CowLib::CowPID(CONSTANT("TURRET_P"), 0, 0, 0);
	m_PID->SetSetpoint(CENTER_X);
}

Turret::~Turret() {

}

void Turret::Handle()
{
	Pixy::PixyPacket packet = Pixy::GetInstance()->GetPacket();

	m_Speed = m_PID->Calculate(packet.x);

	if(m_PID->OnTarget(CONSTANT("TURRET_TOLERANCE")))
	{
		m_PID->ResetIntegrator();
	}

	m_Motor->Set(m_Speed);
}
