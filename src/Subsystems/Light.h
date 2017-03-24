/*
 * Light.h
 *
 *  Created on: Mar 21, 2017
 *      Author: hchau
 */

#ifndef SRC_SUBSYSTEMS_LIGHT_H_
#define SRC_SUBSYSTEMS_LIGHT_H_

#include <WPILib.h>

typedef enum
{
	LIGHT_OFF = 0,
	LIGHT_STROBE,
	LIGHT_ON,
} e_Light_State;

class Light {
public:
	Light(Solenoid *solenoid);
	virtual ~Light();

private:
	Solenoid *m_Solenoid;
	e_Light_State m_State;
	bool m_IsLightOn;
	int m_StrobeCount;
	int m_StrobeRate;
	bool m_Override;

public:
	void Handle();
	void SetState(e_Light_State state);
	void SetLightStrobe();
	void SetLightOn();
	void SetLightStrobeOverride();
	void SetLightOff();
	void SetLightOffOverride();
	void SetStrobeRate(int);
};

#endif /* SRC_SUBSYSTEMS_LIGHT_H_ */
