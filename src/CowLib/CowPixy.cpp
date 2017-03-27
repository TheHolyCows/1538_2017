/*
 * Pixy.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: hchau
 */

#include <CowLib/CowPixy.h>
#include <CowLib/CowLpf.h>
#include "../CowConstants.h"

Pixy *Pixy::m_Instance         = NULL;
SerialPort *Pixy::m_SerialPort = NULL;
AnalogInput *Pixy::m_Analog = NULL;
char Pixy::m_Buf[1024];
Pixy::PixyPacket Pixy::m_Packet;
bool Pixy::m_IsValidPacket = true;

Pixy::Pixy()
{
	m_Analog = new AnalogInput(0);
	m_SerialPort = new SerialPort(9600,
								  SerialPort::kMXP);

	m_Thread = new std::thread(Pixy::Handle);
	memset(m_Buf, 0, 1024);
}

Pixy::~Pixy()
{
	m_Thread->detach();
	delete m_Thread;
}

void Pixy::DoRead()
{
	//std::cout << "Analog: " << m_Analog->GetAverageVoltage() << std::endl;
	m_Packet.analog = m_Analog->GetAverageVoltage();
	/*
	uint32_t readCount = m_SerialPort->Read(m_Buf, 1024);

	if(readCount > 0)
	{
		for(uint32_t i = readCount - 1; i >= 1; i--)
		{
			//printf("0x%x, ", m_Buf[i]);

			uint16_t syncWord = Convert(m_Buf[i], m_Buf[i - 1]);

			if((SYNCWORD == syncWord) && ((i + 12) < readCount))
			{
				uint16_t syncWord2 = Convert(m_Buf[i - 2], m_Buf[i - 3]);
				if(SYNCWORD != syncWord2)
				{
					m_IsValidPacket = false;
					continue;
				}

//				0, 1     y              sync: 0xaa55=normal object, 0xaa56=color code object
//				2, 3     y              checksum (sum of all 16-bit words 2-6, that is, bytes 4-13)
//				4, 5     y              signature number
//				6, 7     y              x center of object
//				8, 9     y              y center of object
//				10, 11   y              width of object
//				12, 13   y              height of object

//				std::cout << std::endl;
//				for(uint8_t j = 0; j < 13 ; j++)
//				{
//					printf("byte %d: 0x%x ,", i+j-1, m_Buf[i+j-1]);
//				}
//				std::cout << std::endl;

				m_IsValidPacket = true;
				uint16_t checksum = Convert(m_Buf[i + 2], m_Buf[i + 1]);
				uint16_t signature = Convert(m_Buf[i + 4], m_Buf[i + 3]);
				m_Packet.x      = Convert(m_Buf[i + 6], m_Buf[i + 5]);
				m_Packet.y      = Convert(m_Buf[i + 8], m_Buf[i + 7]);
				m_Packet.width  = Convert(m_Buf[i + 10], m_Buf[i + 9]);
				m_Packet.height = Convert(m_Buf[i + 12], m_Buf[i + 11]);

				uint16_t sum = signature + m_Packet.x + m_Packet.y + m_Packet.width + m_Packet.height;
				if(checksum != sum)
				{
					continue;
				}
//				std::cout << "signature: " << signature << std::endl;
//				std::cout << std::hex << "checksum: " << checksum << ", sum: "<< sum << std::endl;
//
//				std::cout << std::dec
//						  << "x: " << m_Packet .x
//						  << ", y: " << m_Packet.y
//						  << ", w: " << m_Packet.width
//						  << ", h: " << m_Packet.height << std::endl;
				SmartDashboard::PutNumber("x", m_Packet.x);
				SmartDashboard::PutNumber("y", m_Packet.y);
				SmartDashboard::PutNumber("width", m_Packet.width);
				SmartDashboard::PutNumber("height", m_Packet.height);
				break;
			}
			else
			{
				m_IsValidPacket = true;
			}
		}
		{
			m_IsValidPacket = true;
		}
		memset(m_Buf, 0, 1024);
		*/

	//}
}

void Pixy::Handle()
{
	while(1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		DoRead();
	}

}

Pixy * Pixy::GetInstance()
{
	if(m_Instance == NULL)
	{
		m_Instance = new Pixy();
	}

	return m_Instance;
}

uint16_t Pixy::Convert(char upper, char lower)
{
	uint16_t w1 = (((uint16_t) upper) & 0x00ff) << 8;
	uint16_t w2 = (uint16_t) lower & 0xff;
	return  w1 | w2;
}

Pixy::PixyPacket Pixy::GetPacket()
{
	return m_Packet;
}

bool Pixy::GetPixyPacketValidity()
{
	return m_IsValidPacket;
}
