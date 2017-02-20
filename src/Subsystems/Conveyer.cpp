/*
 * Conveyor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: hchau
 */

#include <Subsystems/Conveyer.h>

Conveyer::Conveyer(uint8_t motorA)
:
	m_MotorA(0),
	m_Speed(0)
{
	m_MotorA = new CANTalon(motorA);
}

Conveyer::~Conveyer() {
	// TODO Auto-generated destructor stub
}

void Conveyer::SetSpeed(float speed)
{
	m_Speed = speed;
}

void Conveyer::Handle()
{
	if(m_MotorA)
	{

		m_MotorA->Set(m_Speed);

	}
}
