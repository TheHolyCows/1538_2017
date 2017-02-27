/*
 * GearIntake.h
 *
 *  Created on: Feb 20, 2017
 *      Author: hchau
 */

#ifndef SRC_SUBSYSTEMS_GEARINTAKE_H_
#define SRC_SUBSYSTEMS_GEARINTAKE_H_

#include "CANTalon.h"

class GearIntake {
private:
	CANTalon *m_MotorArm;
	CANTalon *m_MotorIntake;
	float m_Position;
	float m_Speed;
	float m_Ground;
	float m_GearIntakeTime;

public:
	GearIntake(uint8_t motorArm, uint8_t motorIntake);
	virtual ~GearIntake();
	void SetPosition(float position);
	void SetSpeed(float speed);
	void Handle();
	void DisabledCalibration();
	void ResetConstants();
	float GetGroundOffset();
	float GetCurrent();
	float GetVoltage();
	void SetTime();
};

#endif /* SRC_SUBSYSTEMS_GEARINTAKE_H_ */
