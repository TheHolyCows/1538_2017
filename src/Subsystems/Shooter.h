/*
 * Shooter.h
 *
 *  Created on: Feb 17, 2016
 *      Author: kchau
 */

#ifndef SRC_SUBSYSTEMS_SHOOTER_H_
#define SRC_SUBSYSTEMS_SHOOTER_H_

#include <WPILib.h>
#include "../CowLib/CowPID.h"
#include "../CowLib/CowLPF.h"

class Shooter {
private:
	Talon* m_MotorA;
	float m_Speed;

	Encoder *m_EncoderA;

	CowLib::CowPID* m_PID_A_Rate;

	CowLib::CowLPF* m_LPF_A;


	bool m_PIDEnabled;
	double m_EncoderASpeed;
	Shooter();
public:
	Shooter(uint8_t motorA, Encoder* encoderA);
	void SetManualSpeed(float speed);
	void SetAutoSpeed(float speedA);
	void SetPIDState(bool state);
	double GetMotorASpeed();
	double GetMotorBSpeed();
	bool IsOnTarget();
	bool HasShotBall();
	void ResetConstants();
	void Reset();
	void Handle();
	virtual ~Shooter();
};

#endif /* SRC_SUBSYSTEMS_SHOOTER_H_ */
