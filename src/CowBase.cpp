#include "CowBase.h"
#include <string.h>

CowBase::CowBase()
	:
	m_ControlBoard(new CowControlBoard()),
	m_OpController(new OperatorController(m_ControlBoard)),
	m_AutoController(new AutoModeController()),
	m_Constants(CowConstants::GetInstance())
{	
	CowConstants::GetInstance()->RestoreData();
	m_Bot = new CowRobot();

	m_Display = new CowDisplay(m_Bot);

	//SetPeriod(HZ(ROBOT_HZ));
	//GetWatchdog().SetEnabled(false);
	printf("Done constructing CowBase!\n");
}

CowBase::~CowBase()
{
	delete m_ControlBoard;
	delete m_OpController;
	delete m_AutoController;
	delete m_Display;
}

void CowBase::RobotInit()
{
	system("dhclient -r eth0");
	m_Bot->Reset();
	//m_Bot->GetArm()->SetLockState(true);
}

void CowBase::DisabledInit()
{
	CowConstants::GetInstance()->RestoreData();
	m_Bot->GetGyro()->BeginCalibration();
	m_Bot->Reset();

}

void CowBase::AutonomousInit()
{
	m_Bot->GetGyro()->FinalizeCalibration();
	m_Bot->GetGyro()->ResetAngle();
	m_AutoController->SetCommandList(AutoModes::GetInstance()->GetCommandList());
	m_Bot->SetController(m_AutoController);
	m_Bot->Reset();
}
void CowBase::TeleopInit()
{
	m_Bot->StartTime();
	m_Bot->GetGyro()->FinalizeCalibration();
	m_Bot->SetController(m_OpController);
}

void CowBase::DisabledContinuous()
{
	//taskDelay(WAIT_FOREVER);
}

void CowBase::AutonomousContinuous()
{
	//taskDelay(WAIT_FOREVER);
}

void CowBase::TeleopContinuous()
{
	//taskDelay(WAIT_FOREVER);
}

void CowBase::DisabledPeriodic()
{
	//m_Bot->GyroHandleCalibration();
	if(m_Display)
	{
		m_Display->DisplayPeriodic();
	}
	if(m_ControlBoard->GetAutoSelectButton())
	{
		m_Constants->RestoreData();
		m_Bot->GetShooter()->ResetConstants();

		if(m_ControlBoard->GetDriveButton(1))
		{
			m_Bot->Reset();
			AutoModes::GetInstance()->NextMode();
		}
	}
	
	if(m_ControlBoard->GetAutoAddAngleOffsetButton())
	{
		if(m_ControlBoard->GetSteeringButton(9))
		{
			m_Bot->AddAutoOffsetAngle();
		}
	}
	if(m_ControlBoard->GetAutoDecAngleOffsetButton())
	{
		if(m_ControlBoard->GetSteeringButton(11))
		{
			m_Bot->DecAutoOffsetAngle();
		}
	}

	if(m_ControlBoard->GetSteeringButton(7))
	{
		std::string temp = "Using Offset Position";
		DriverStation::GetInstance().ReportError(temp);
	}

//	m_Bot->PrintToDS();
}
void CowBase::AutonomousPeriodic()
{
	m_Bot->handle();
}
void CowBase::TeleopPeriodic()
{
	m_Bot->handle();
	if(m_Display)
	{
		m_Display->DisplayPeriodic();
	}
}

START_ROBOT_CLASS(CowBase);
