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

	m_Modes["Gear N Shoot"];
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 70, 0, 0.375, true, 1.25));
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, 180, -65, 0.375, true, 2));
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 180, -65, 0.375, true, 1));
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_HOLD_DISTANCE, 180, -65, 0.375, false, 0.25));
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_RESET_DRIVE_ENCODER, 0, 0, 0, false, 0.025));
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, -24, -65, 0.55, false, 0.5));
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_DRIVE_DISTANCE, -48, -100, 0.55, true, 0.5));
	m_Modes["Gear N Shoot"].push_back(RobotCommand(CMD_HOLD_DISTANCE, -52, -90, 0.55, true, 3));


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
