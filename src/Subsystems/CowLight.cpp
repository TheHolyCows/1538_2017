/*
 * CowLight.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: hchau
 */

#include <Subsystems/CowLight.h>
#include <WPILib.h>

CowLight::CowLight(Solenoid *solenoid) :
	m_State(LIGHT_OFF),
	m_IsLightOn(false),
	m_Solenoid(solenoid)
{
	// TODO Auto-generated constructor stub

}

CowLight::~CowLight() {
	// TODO Auto-generated destructor stub
}

void CowLight::SetState(e_Light_State state)
{
	m_State = state;
}

void CowLight::SetLightStrobe()
{
	SetState(LIGHT_STROBE);
}

void CowLight::SetLightOn()
{
	SetState(LIGHT_ON);
}

void CowLight::SetLightOff()
{
	SetState(LIGHT_OFF);
}

void CowLight::Handle()
{
	switch(m_State)
	{
		case LIGHT_OFF:
		{
			m_Solenoid->Set(false);
			m_IsLightOn = false;
			break;
		}
		case LIGHT_STROBE:
		{
			if(m_IsLightOn)
			{
				m_Solenoid->Set(false);
			}
			else
			{
				m_Solenoid->Set(true);
			}

			m_IsLightOn = !m_IsLightOn;

			break;
		}
		case LIGHT_ON:
		{
			m_Solenoid->Set(true);
			m_IsLightOn = true;
			break;
		}
	}
}
