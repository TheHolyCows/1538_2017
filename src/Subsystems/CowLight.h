/*
 * CowLight.h
 *
 *  Created on: Mar 21, 2017
 *      Author: hchau
 */

#ifndef SRC_SUBSYSTEMS_COWLIGHT_H_
#define SRC_SUBSYSTEMS_COWLIGHT_H_

#include <WPILib.h>

typedef enum
{
	LIGHT_OFF = 0,
	LIGHT_STROBE,
	LIGHT_ON,
} e_Light_State;

class CowLight {
public:
	CowLight(Solenoid *solenoid);
	virtual ~CowLight();

private:
	Solenoid *m_Solenoid;
	e_Light_State m_State;
	bool m_IsLightOn;

public:
	void Handle();
	void SetState(e_Light_State state);
	void SetLightStrobe();
	void SetLightOn();
	void SetLightOff();
};

#endif /* SRC_SUBSYSTEMS_COWLIGHT_H_ */
