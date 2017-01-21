/*
 * Conveyor.h
 *
 *  Created on: Jan 21, 2017
 *      Author: hchau
 */

#ifndef SRC_SUBSYSTEMS_CONVEYER_H_
#define SRC_SUBSYSTEMS_CONVEYER_H_

#include <WPILib.h>

class Conveyer {
private:
	Talon* m_MotorA;
	float m_Speed;

public:
	Conveyer(uint8_t motorA);
	virtual ~Conveyer();
	void SetSpeed(float speed);
	void Handle();
};

#endif /* SRC_SUBSYSTEMS_CONVEYOR_H_ */
