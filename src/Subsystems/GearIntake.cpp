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
	m_Ground(0),
	m_GearIntakeTime(0),
	m_IntakeAfterRaise(false),
	m_IntakeAfterRaiseTime(0)
{
	m_MotorArm = new CANTalon(motorArm);
	m_MotorArm->SetSensorDirection(true);
	m_MotorIntake = new CANTalon(motorIntake);

	m_MotorArm->SetControlMode(CANTalon::kPosition);
	m_MotorArm->SetVoltageRampRate(CONSTANT("ARM_VOLT_RAMP"));
	//m_MotorArm->SetCloseLoopRampRate(CONSTANT("ARM_VOLT_RAMP"));
	m_MotorArm->SetPID(CONSTANT("ARM_P"), CONSTANT("ARM_I"), CONSTANT("ARM_D"), 0);

	m_TimeLatch = new CowLib::CowLatch();
}

GearIntake::~GearIntake() {
	// TODO Auto-generated destructor stub
}

void GearIntake::SetPosition(float position)
{
	if(m_TimeLatch->Latch((position == CONSTANT("ARM_UP"))))
	{
		if(m_IntakeAfterRaise)
		{
			m_IntakeAfterRaiseTime = Timer::GetFPGATimestamp();
			printf("Setting Time Latch\r\n");
		}
	}
	else if(position != CONSTANT("ARM_UP"))
	{
		m_TimeLatch->ResetLatch();
	}

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
	float intakeSpeed = m_Speed;
	float intakeTime = (Timer::GetFPGATimestamp() - m_IntakeAfterRaiseTime);

//	if(m_IntakeAfterRaise)
//	{
//		if(intakeTime < CONSTANT("INTAKE_BLIP_TIME"))
//		{
//			intakeSpeed = 1;
//		}
//		else
//		{
//			m_IntakeAfterRaise = false;
//		}
//	}

	m_MotorIntake->Set(intakeSpeed);
	m_MotorArm->SetSetpoint(m_Position);

	//m_MotorArm->Set(m_Position);
	//std::cout << "GeartIntake sp: " << m_Position << ", offset: " << m_Ground << ", pos: " << m_MotorArm->GetPosition() << std::endl;

	//double watts = m_MotorIntake->GetOutputCurrent()  * m_MotorIntake->GetOutputVoltage();
	//std::cout << "GearIntake watts:" << watts << std::endl;

//	if((m_Position >= (GetGroundOffset() + CONSTANT("ARM_MIN"))) &&
//	   ((Timer::GetFPGATimestamp() - m_GearIntakeTime) > CONSTANT("INTAKE_TIME")))
//	{
//		printf("Checking watts: %f\r\n", (GetCurrent() * GetVoltage()));
//		if((GetCurrent() * GetVoltage()) > CONSTANT("INTAKE_WATTS"))
//		{
//			SetPosition(CONSTANT("ARM_UP"));
//		}
//	}
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

float GearIntake::GetCurrent()
{
	return m_MotorIntake->GetOutputCurrent();
}

float GearIntake::GetVoltage()
{
	return m_MotorIntake->GetOutputVoltage();
}

void GearIntake::SetTime()
{
	m_GearIntakeTime = Timer::GetFPGATimestamp();
}

void GearIntake::IntakeAfterRaise()
{
	m_IntakeAfterRaise = true;
}
