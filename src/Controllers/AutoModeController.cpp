#include "AutoModeController.h"

AutoModeController::AutoModeController()
	:
	m_Timer(new Timer()),
	m_CurrentCommand(RobotCommand())
{
	m_Timer->Start();
	reset();
}

void AutoModeController::SetCommandList(deque<RobotCommand> list)
{
	m_CommandList = list;
}

void AutoModeController::reset()
{
	CowConstants::GetInstance();

	m_CommandList.clear();
	m_CurrentCommand = RobotCommand();
}


void AutoModeController::handle(CowRobot *bot)
{
	bool result = false;
	
	// Run the command
	switch(m_CurrentCommand.m_Command)
	{
		case CMD_NULL:
		{
			doNothing(bot);
			if(m_CurrentCommand.m_Arm)
			{
				bot->GetGearIntake()->SetPosition(CONSTANT("ARM_UP"));
			}
			else
			{
				bot->GetGearIntake()->SetPosition(bot->GetGearIntake()->GetGroundOffset());
			}
			result = true;
			break;
		}
		case CMD_WAIT:
		{
			bot->DriveWithHeading(m_CurrentCommand.m_Heading, 0);
			doNothing(bot);
			if(m_CurrentCommand.m_Arm)
			{
				bot->GetGearIntake()->SetPosition(CONSTANT("ARM_UP"));
			}
			else
			{
				bot->GetGearIntake()->SetPosition(bot->GetGearIntake()->GetGroundOffset());
			}
			break;
		}
		case CMD_TURN:
		{
			result = bot->TurnToHeading(m_CurrentCommand.m_Heading);
			if(m_CurrentCommand.m_Arm)
			{
				bot->GetGearIntake()->SetPosition(CONSTANT("ARM_UP"));
			}
			else
			{
				bot->GetGearIntake()->SetPosition(bot->GetGearIntake()->GetGroundOffset());
			}

			break;
		}
		case CMD_HOLD_DISTANCE:
		{
			bot->DriveDistanceWithHeading(m_CurrentCommand.m_Heading, m_CurrentCommand.m_EncoderCount, m_CurrentCommand.m_Speed);
			if(m_CurrentCommand.m_Arm)
			{
				bot->GetGearIntake()->SetPosition(CONSTANT("ARM_UP"));
			}
			else
			{
				bot->GetGearIntake()->SetPosition(bot->GetGearIntake()->GetGroundOffset());
			}
			result = false;
			break;
		}
		case CMD_DRIVE_DISTANCE:
		{
			result = bot->DriveDistanceWithHeading(m_CurrentCommand.m_Heading, m_CurrentCommand.m_EncoderCount,  m_CurrentCommand.m_Speed );
			if(m_CurrentCommand.m_Arm)
			{
				bot->GetGearIntake()->SetPosition(CONSTANT("ARM_UP"));
			}
			else
			{
				bot->GetGearIntake()->SetPosition(bot->GetGearIntake()->GetGroundOffset());
			}
			break;
		}
		case CMD_RESET_DRIVE_ENCODER:
		{
			break;
		}
		default:
		{
			doNothing(bot);
			bot->GetGearIntake()->SetPosition(CONSTANT("ARM_UP"));

			result = true;
			break;
		}
	}
	
	// Check if this command is done
	if(result == true || m_CurrentCommand.m_Command == CMD_NULL || m_Timer->Get() > m_CurrentCommand.m_Timeout)
	{
		// This command is done, go get the next one
		if(m_CommandList.size() > 0 )
		{
			if(m_CurrentCommand.m_Command == CMD_TURN)
			{
				bot->GetEncoder()->Reset();
			}
			m_CurrentCommand = m_CommandList.front();
			m_CommandList.pop_front();
			//bot->GetEncoder()->Reset();
			
			if(!m_CurrentCommand.m_Command == CMD_NULL)
			{
				printf("Time elapsed: %f\n", m_Timer->Get());
			}
		}
		else
		{
			//we're done clean up
			m_CurrentCommand = RobotCommand();
		}
		m_Timer->Reset();
	}
}

// Drive Functions
void AutoModeController::doNothing(CowRobot *bot)
{
	bot->DriveLeftRight(0, 0);
}
