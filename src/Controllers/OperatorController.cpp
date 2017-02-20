#include "OperatorController.h"

OperatorController::OperatorController(CowControlBoard *controlboard)
	:
	m_CB(controlboard)
{
	m_ShootLatch = new CowLib::CowLatch();
	m_SpoolShooterLatch = new CowLib::CowLatch();
	time = 0;
}

void OperatorController::handle(CowRobot *bot)
{
	bot->DriveSpeedTurn(m_CB->GetDriveStickY(),
						m_CB->GetSteeringX(),
						m_CB->GetSteeringButton(FAST_TURN));

	// Turn on Shooter
	if(m_SpoolShooterLatch->Latch(!m_CB->GetOperatorButton(1)))
	{
		std::cout << "Setting state to SPOOL_PID_CONTROL" << std::endl;
		bot->GetShooter()->SetPIDState(true);
//		// start shooter
		bot->GetShooter()->SetAutoSpeed(CONSTANT("SHOOTER_RPM"));

		//bot->GetShooter()->SetManualSpeed();
	}
	else if(m_CB->GetOperatorButton(1))
	{
		m_SpoolShooterLatch->ResetLatch();
		bot->GetShooter()->SetPIDState(false);
		bot->GetShooter()->SetAutoSpeed(0);
	}

	if (m_CB->GetOperatorButton(5))
	{
		bot->GetShooter()->SetFeederSpeed(-1);
		bot->GetConveyerUpper()->SetSpeed(-1);
		bot->GetConveyerLower()->SetSpeed(-1);
	}
	else
	{
		bot->GetShooter()->SetFeederSpeed(0);
		bot->GetConveyerUpper()->SetSpeed(0);
		bot->GetConveyerLower()->SetSpeed(0);
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

	newSetPoint = bot->GetTurret()->GetSetPoint() + (CowLib::Deadband(m_CB->GetOperatorGamepadAxis(0), 0.1) * 2000);
	bot->GetTurret()->SetSetPoint(newSetPoint);
}

