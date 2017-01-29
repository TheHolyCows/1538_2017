/*
 * Pixy.h
 *
 *  Created on: Jan 28, 2017
 *      Author: hchau
 */

#ifndef SRC_COWLIB_COWPIXY_H_
#define SRC_COWLIB_COWPIXY_H_

#include <WPILib.h>
#include <thread>

class Pixy {
public:
	struct PixyPacket
	{
		uint16_t x;
		uint16_t y;
		uint16_t width;
		uint16_t height;
	};

private:
	static const uint16_t SYNCWORD = 0xaa55;

	static SerialPort *m_SerialPort;
	static Pixy *m_Instance;
	static char m_Buf[1024];
	static PixyPacket m_Packet;


	std::thread *m_Thread;

public:
	static Pixy *GetInstance();
	static PixyPacket GetPacket();

private:
	Pixy();
	virtual ~Pixy();
	static void Handle();
	static void DoRead();
	static uint16_t Convert(char upper, char lower);
};

#endif /* SRC_COWLIB_COWPIXY_H_ */
