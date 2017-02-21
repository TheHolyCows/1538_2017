/*
 * GearIntake.cpp
 *
 *  Created on: Feb 20, 2017
 *      Author: hchau
 */

#include <Subsystems/GearIntake.h>
#include "../CowConstants.h"
#include <CANTalon.h>

GearIntake::GearIntake(uint8_t motorArm, uint8_t motorIntake) :
	m_MotorArm(0),
	m_MotorIntake(0),
	m_Speed(0),
	m_Position(0),
	m_Ground(0)
{
	m_MotorArm = new CANTalon(motorArm);
	m_MotorArm->SetSensorDirection(true);
	m_MotorIntake = new CANTalon(motorIntake);

	m_MotorArm->SetControlMode(CANTalon::kPosition);
	m_MotorArm->SetVoltageRampRate(CONSTANT("ARM_VOLT_RAMP"));
	//m_MotorArm->SetCloseLoopRampRate(CONSTANT("ARM_VOLT_RAMP"));
	m_MotorArm->SetPID(CONSTANT("ARM_P"), CONSTANT("ARM_I"), CONSTANT("ARM_D"), 0);
}

GearIntake::~GearIntake() {
	// TODO Auto-generated destructor stub
}

void GearIntake::SetPosition(float position)
{
	// red goes up
	// green goes down
	float newPosition = position + m_Ground;
	if(newPosition < CONSTANT("ARM_MAX") + m_Ground)
	{
		m_Position = CONSTANT("ARM_MAX") + m_Ground;
	}
	else if(newPosition > CONSTANT("ARM_MIN") + m_Ground)
	{
		m_Position = CONSTANT("ARM_MIN") + m_Ground;
	}
	else
	{
		m_Position = newPosition;
	}
}

void GearIntake::SetSpeed(float speed)
{
	m_Speed = speed;
}

void GearIntake::Handle()
{
	m_MotorIntake->Set(m_Speed);
	m_MotorArm->SetSetpoint(m_Position);
	//m_MotorArm->Set(m_Position);
	std::cout << "GeartIntake sp: " << m_Position << ", offset: " << m_Ground << ", pos: " << m_MotorArm->GetPosition() << std::endl;
}

void GearIntake::DisabledCalibration()
{
	float newPosition = m_MotorArm->GetPosition();
	std::cout << "GeartIntake position: " << newPosition << ", offset: " << m_Ground << std::endl;
	if(m_Ground < newPosition)
	{
		m_Ground = newPosition;
	}
}

void GearIntake::ResetConstants()
{
	m_MotorArm->SetVoltageRampRate(CONSTANT("ARM_VOLT_RAMP"));
	//m_MotorArm->SetCloseLoopRampRate(CONSTANT("ARM_VOLT_RAMP"));
	m_MotorArm->SetPID(CONSTANT("ARM_P"), CONSTANT("ARM_I"), CONSTANT("ARM_D"), 0);
}

float GearIntake::GetGroundOffset()
{
	return m_Ground;
}