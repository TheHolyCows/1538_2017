/*
 * Turret.h
 *
 *  Created on: Jan 29, 2017
 *      Author: hchau
 */

#ifndef SRC_SUBSYSTEMS_TURRET_H_
#define SRC_SUBSYSTEMS_TURRET_H_

#include <WPILib.h>
#include "../CowLib/CowPID.h"

class Turret {
private:
	Talon* m_Motor;
	CowLib::CowPID* m_PID;
	float m_Speed;

	const uint32_t CENTER_X = 319 / 2;	// width ranges from 0 to 319
	const uint32_t CENTER_Y = 199 / 2;	// height ranges from 0 to 199

public:
	Turret(uint8_t motor);
	virtual ~Turret();
	void Handle();
};

#endif /* SRC_SUBSYSTEMS_TURRET_H_ */
