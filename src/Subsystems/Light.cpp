/*
 * Light.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: hchau
 */

#include <Subsystems/Light.h>
#include <WPILib.h>

Light::Light(Solenoid *solenoid) :
	m_State(LIGHT_OFF),
	m_IsLightOn(false),
	m_Solenoid(solenoid),
	m_StrobeCount(0)
{
	// TODO Auto-generated constructor stub

}

Light::~Light() {
	// TODO Auto-generated destructor stub
}

void Light::SetState(e_Light_State state)
{
	m_State = state;
}

void Light::SetLightStrobe()
{
	SetState(LIGHT_STROBE);
}

void Light::SetLightOn()
{
	SetState(LIGHT_ON);
}

void Light::SetLightOff()
{
	SetState(LIGHT_OFF);
}

void Light::Handle()
{
	switch(m_State)
	{
		case LIGHT_OFF:
		{
			m_Solenoid->Set(false);
			m_IsLightOn = false;
			m_StrobeCount = 0;
			break;
		}
		case LIGHT_STROBE:
		{
			m_StrobeCount++;

			if (m_StrobeCount == 2)
			{
				m_StrobeCount = 0;
			}
			else
			{
				break;
			}

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
			m_StrobeCount = 0;
			break;
		}
	}
}
