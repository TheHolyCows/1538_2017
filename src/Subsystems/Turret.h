/*
 * Turret.h
 *
 *  Created on: Jan 29, 2017
 *      Author: hchau
 */

#ifndef SRC_SUBSYSTEMS_TURRET_H_
#define SRC_SUBSYSTEMS_TURRET_H_

#include <WPILib.h>
#include <CANTalon.h>
#include "../CowLib/CowPID.h"
#include "../CowLib/CowLPF.h"
#include "../CowPixyBlock.h"

class Turret {
private:
	CANTalon* m_Motor;
	Solenoid *m_PixySolenoid;

	float m_Speed;
	float m_PositionMax;
	float m_PositionMin;
	float m_SetPoint;
	float m_LastPixyValue;
	bool m_OperatorOverride;

	double m_PixyCenterX;
	double m_PixyScaleFactor;
	double m_LightTime;
	CowLib::CowLPF *m_Lpf;
	bool m_AutoTurret;
	//const uint32_t CENTER_X = 319 / 2;	// width ranges from 0 to 319
	//const uint32_t CENTER_Y = 199 / 2;	// height ranges from 0 to 199

public:
	Turret(uint8_t motor);
	virtual ~Turret();
	void SetAutoTurret(bool turret);
	void Handle();
	void SetSpeed(float speed);
	void ResetConstants();
	void SetSetPoint(float sp);
	bool IsOnTarget();
	float GetSetPoint();
	void SetTurretOperatorOverride(bool override);
};

#endif /* SRC_SUBSYSTEMS_TURRET_H_ */
