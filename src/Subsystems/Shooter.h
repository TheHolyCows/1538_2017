/*
 * Shooter.h
 *
 *  Created on: Feb 17, 2016
 *      Author: kchau
 */

#ifndef SRC_SUBSYSTEMS_SHOOTER_H_
#define SRC_SUBSYSTEMS_SHOOTER_H_

#include <WPILib.h>
#include <CANTalon.h>
#include "../CowLib/CowPID.h"
#include "../CowLib/CowLPF.h"

class Shooter {
private:
	CANTalon *m_MotorA;
	CANTalon *m_MotorB;
	CANTalon *m_MotorFeeder;
	float m_Speed;
	bool m_PIDEnabled;
	CowLib::CowLPF *m_Lpf;


	Shooter();
public:
	Shooter(uint32_t motorA, uint32_t motorB, uint32_t motorFeeder);
	void SetManualSpeed(float speed);
	void SetAutoSpeed(float speedA);
	void SetPIDState(bool state);
	double GetSpeed();
	bool IsOnTarget();
	bool HasShotBall();
	void SetFeederSpeed(float speed);
	void ResetConstants();
	void Reset();
	void Handle();
	virtual ~Shooter();
	void SetDashboard(SmartDashboard *dashboard);
};

#endif /* SRC_SUBSYSTEMS_SHOOTER_H_ */
