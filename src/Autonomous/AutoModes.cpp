#include "AutoModes.h"
#include <WPILib.h>

AutoModes *AutoModes::m_SingletonInstance = NULL;

AutoModes *AutoModes::GetInstance()
{
	if(m_SingletonInstance == NULL)
	{
		m_SingletonInstance = new AutoModes();
	}
	return m_SingletonInstance;
}

AutoModes::AutoModes()
{	
	// Set up our selection order
	m_Modes["Nothing"];
	m_Modes["Nothing"].push_back(RobotCommand(CMD_WAIT, 0, 0, 0, true, 0));

	m_Modes["Gear Straight - Left"];
	m_Modes["Gear Straight - Left"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 80, 0, 0.375, true, 2));
	m_Modes["Gear Straight - Left"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 75, 0, 0.375, false, 1));
	m_Modes["Gear Straight - Left"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 45, 0, 0.375, false, 1));
	m_Modes["Gear Straight - Left"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 10, 45, 0.5, true, 0.5));
	m_Modes["Gear Straight - Left"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, -125, 90, 0.5, true, 1.25));
	m_Modes["Gear Straight - Left"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, -250, 180, 0.5, true, 2));

	m_Modes["Gear Straight - Right"];
	m_Modes["Gear Straight - Right"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 80, 0, 0.375, true, 2));
	m_Modes["Gear Straight - Right"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 75, 0, 0.375, false, 1));
	m_Modes["Gear Straight - Right"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 45, 0, 0.375, false, 1));
	m_Modes["Gear Straight - Right"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 10, -45, 0.5, true, 0.5));
	m_Modes["Gear Straight - Right"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, -25, -90, 0.5, true, 1.25));
	m_Modes["Gear Straight - Right"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, -250, -180, 0.5, true, 2));


	m_Modes["Gear N Shoot - Right"];
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 70, 0, 0.375, true, 1.25));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 180, -60, 0.375, true, 2));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 180, -60, 0.375, true, 1));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 180, -60, 0.375, false, 0.25));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 180, -60, 0.375, false, 0.25));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_SPOOL_SHOOTER, 0, 0, 0, false, 0.025));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 80, -98, 0.55, false, 1));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 80, -98, 0.375, true, 0.5));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 70, -90, 0.5, true, 0.5));
	m_Modes["Gear N Shoot - Right"].push_back(RobotCommand(CMD_SHOOT, 70, -90, 0.5, true, 15));

	m_Modes["Gear N Shoot - Left"];
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 70, 0, 0.375, true, 1.25));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 196, 60, 0.375, true, 2));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 196, 60, 0.375, true, 1));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 196, 60, 0.375, false, 0.25));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 196, 60, 0.375, false, 0.25));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_SPOOL_SHOOTER, 0, 0, 0, false, 0.025));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 70, 98, 0.55, false, 1));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 70, 98, 0.375, true, 0.5));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 60, 90, 0.5, true, 0.5));
	m_Modes["Gear N Shoot - Left"].push_back(RobotCommand(CMD_SHOOT, 60, 90, 0.5, true, 15));



	m_Iterator = m_Modes.begin();
}

std::deque<RobotCommand> AutoModes::GetCommandList()
{
	return m_Iterator->second;
}

const char *AutoModes::GetName()
{
	return m_Iterator->first;
}

void AutoModes::NextMode()
{
	++m_Iterator;
	if(m_Iterator == m_Modes.end())
	{
		m_Iterator = m_Modes.begin();
	}
	std::string str(GetName());
	std::string temp = "Auto mode: " + str;
	DriverStation::GetInstance().ReportError(temp);
}
