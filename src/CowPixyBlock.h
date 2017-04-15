/*
 * PixyBlock.h
 *
 *  Created on: Apr 3, 2017
 *      Author: klai
 */

#ifndef SRC_COWLIB_COWPIXYBLOCK_H_
#define SRC_COWLIB_COWPIXYBLOCK_H_

#include <WPILib.h>
#include <thread>

// Most of this code was from ChiefDelphi which was taken from 599 which was taken from Arduino
#define PIXY_INITIAL_ARRAYSIZE 30
#define PIXY_MAXIMUM_ARRAYSIZE 130
#define PIXY_START_WORD        0xaa55 // For regular color recognition
#define PIXY_START_WORD_CC     0xaa56 // For color code - angle rotation recognition
#define PIXY_START_WORDX       0x55aa // Regular color another way around
#define PIXY_MAX_SIGNATURE     7
#define PIXY_DEFAULT_ARGVAL    0xffff

#define DEFAULT_BLOCK_NUM 100

class PixyBlock {
public:

private:
	enum BlockType
	{
	   NORMAL_BLOCK, //normal color recognition
	   CC_BLOCK     //color-code(chnage in angle) recognition
	};

	struct Block
	{
		uint16_t signature; // Identification number for your object - you could set it in the PixyMon
		uint16_t x; // 0 - 320
		uint16_t y; // 0 - 200
		uint16_t width;
		uint16_t height;
		uint16_t angle;
	};

	static I2C *m_I2C;
	static BlockType m_BlockType;
	static PixyBlock *m_Instance;
	static Block m_Blocks[DEFAULT_BLOCK_NUM];
	static int m_LastX;
	static int m_LastY;

	static double m_Timestamp;
	static int m_Count;
	static bool m_EnableRead;

	std::thread *m_Thread;

public:
	static PixyBlock *GetInstance();
	static void Handle();
	static void DoRead();
	static int GetX();
	static int GetY();
	static void SetRead(bool val);

private:
	PixyBlock();
	virtual ~PixyBlock();
	static uint8_t GetByte();
	static uint16_t GetWord();
	static bool GetStart();
	static uint16_t GetBlocks(uint16_t);
	void Print(Block *);
};

#endif /* SRC_COWLIB_COWPIXYBLOCK_H_ */
