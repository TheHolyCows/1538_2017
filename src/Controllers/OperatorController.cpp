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
		// start shooter
		bot->GetShooter()->SetAutoSpeed(CONSTANT("SHOOTER_RPM_A"));
	}
	else if(m_CB->GetOperatorButton(1))
	{
		m_SpoolShooterLatch->ResetLatch();
		bot->GetShooter()->SetPIDState(false);
		bot->GetShooter()->SetManualSpeed(0);
	}

}

