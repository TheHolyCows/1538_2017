#include "OperatorController.h"

OperatorController::OperatorController(CowControlBoard *controlboard)
	:
	m_CB(controlboard)
{
	m_ShootLatch = new CowLib::CowLatch();
	m_SpoolShooterLatch = new CowLib::CowLatch();
	m_GearIntakeLatch = new CowLib::CowLatch();
	time = 0;
}

void OperatorController::handle(CowRobot *bot)
{
	if(m_CB->GetOperatorButton(3))
	{
		bot->GetClimber()->Set(1);
	}
	else
	{
		bot->GetClimber()->Set(0);
	}
	// Ball intake and exhaust
	//Exhaust
	if (m_CB->GetOperatorButton(9))
	{
		bot->GetBallIntakeConveyer()->SetSpeed(1);
	}
	else if (m_CB->GetOperatorButton(7))
	{
		bot->GetBallIntakeConveyer()->SetSpeed(-1);
	}
	else
	{
		bot->GetBallIntakeConveyer()->SetSpeed(0);
	}

	// Turn on Shooter
	if(m_SpoolShooterLatch->Latch(!m_CB->GetOperatorButton(10)))
	{
		std::cout << "Setting state to SPOOL_PID_CONTROL" << std::endl;
		bot->GetShooter()->SetPIDState(true);
//		// start shooter
		bot->GetShooter()->SetAutoSpeed(CONSTANT("SHOOTER_RPM"));
		bot->GetTurret()->SetAutoTurret(true);
		//bot->GetShooter()->SetManualSpeed();
	}
	else if(m_CB->GetOperatorButton(10))
	{
		m_SpoolShooterLatch->ResetLatch();
		bot->GetShooter()->SetPIDState(false);
		bot->GetShooter()->SetAutoSpeed(0);
		bot->GetTurret()->SetAutoTurret(false);
	}

	// shoot
	if ((m_CB->GetOperatorButton(5) || (bot->GetTurret()->IsOnTarget() && CONSTANT("AUTO_FIRE") != 0 && fabs(m_CB->GetDriveStickY()) < CONSTANT("AUTO_FIRE_JOYSTICK"))))
	{
		bot->GetShooter()->SetFeederSpeed(-1);
		bot->GetConveyerUpper()->SetSpeed(-1);
		bot->GetConveyerLower()->SetSpeed(-0.75);
		bot->GetBallIntakeConveyer()->SetSpeed(-1);
	}
	else
	{
		bot->GetShooter()->SetFeederSpeed(0);
		bot->GetConveyerUpper()->SetSpeed(0);
		bot->GetConveyerLower()->SetSpeed(0);
	}

	if(m_CB->GetDriveButton(1))
	{
		//bot->TurnToHeading(90);
		bot->DriveDistanceWithHeading(0, 12, 0.5);
	}
	else
	{
		bot->DriveSpeedTurn(m_CB->GetDriveStickY(),
							m_CB->GetSteeringX(),
							m_CB->GetSteeringButton(FAST_TURN));
	}

//	// Turn on Conveyer
//	if(m_CB->GetOperatorButton(5) && bot->GetShooter()->IsOnTarget())
//	{
//		bot->GetConveyer()->SetSpeed(CONSTANT("CONVEYER_A"));
//	}
//	else if(m_CB->GetOperatorButton(7))
//	{
//		bot->GetConveyer()->SetSpeed(-1);
//	}
//	else
//	{
//		bot->GetConveyer()->SetSpeed(0);
//	}

	// Exhaust

	// Turret
	// red goes right
	// green goes left
	//bot->GetTurret()->SetSpeed(m_CB->GetOperatorGamepadAxis(0));

	float newSetPoint;

//	newSetPoint = bot->GetTurret()->GetSetPoint() + (CowLib::Deadband(m_CB->GetOperatorGamepadAxis(0), 0.1) * 2000);
//	bot->GetTurret()->SetSetPoint(newSetPoint);

	//bot->GetGearIntake()->SetPosition(CowLib::Deadband(m_CB->GetOperatorGamepadAxis(2), 0.1));

	// Gear Intake Arm up and down
	if(m_GearIntakeLatch->Latch(!m_CB->GetOperatorButton(8)))
	{
		std::cout << "Latch GearIntake" << std::endl;
		bot->GetGearIntake()->SetPosition(bot->GetGearIntake()->GetGroundOffset());
	}
	else if(m_CB->GetOperatorButton(8))
	{
		m_GearIntakeLatch->ResetLatch();
		bot->GetGearIntake()->SetPosition(CONSTANT("ARM_UP"));
	}

	// Gear intake and exhaust
	if (m_CB->GetOperatorButton(4))
	{
		bot->GetGearIntake()->SetSpeed(CONSTANT("GEAR_INTAKE_SPEED"));
		//bot->GetGearIntake()->IntakeAfterRaise();
		//bot->GetGearIntake()->SetTime();
	}
	else if (m_CB->GetOperatorButton(6))
	{
		bot->GetGearIntake()->SetSpeed(CONSTANT("GEAR_EXHAUST_SPEED"));
	}
	else
	{
		bot->GetGearIntake()->SetSpeed(0);
	}

	// Light
	if (m_CB->GetSteeringButton(1) ||
		m_CB->GetSteeringButton(2) ||
		m_CB->GetSteeringButton(3) ||
		m_CB->GetSteeringButton(4) ||
		m_CB->GetOperatorButton(3))
	{
		bot->GetLight()->SetLightStrobe();
	}
	else if (!m_CB->GetSteeringButton(1) &&
			 !m_CB->GetSteeringButton(2) &&
			 !m_CB->GetSteeringButton(3) &&
			 !m_CB->GetSteeringButton(4) &&
			 !m_CB->GetOperatorButton(3))
	{
		bot->GetLight()->SetLightOff();
	}
}

