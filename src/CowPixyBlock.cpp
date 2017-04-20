/*
 * PixyBlock.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: klai
 */

#include "CowPixyBlock.h"

PixyBlock *PixyBlock::m_Instance = NULL;
I2C *PixyBlock::m_I2C = NULL;
int PixyBlock::m_LastX = 0;
int PixyBlock::m_LastY = 0;
PixyBlock::Block PixyBlock::m_Blocks[DEFAULT_BLOCK_NUM];
PixyBlock::BlockType PixyBlock::m_BlockType = PixyBlock::NORMAL_BLOCK;
double PixyBlock::m_Timestamp = 0;
int PixyBlock::m_Count = 0;
bool PixyBlock::m_EnableRead = false;

PixyBlock::PixyBlock()
{
	m_I2C = new I2C(I2C::Port::kMXP, 0x56);
	m_Thread = new std::thread(PixyBlock::Handle);
}

PixyBlock::~PixyBlock()
{
	m_Thread->detach();
	delete m_Thread;
}

void PixyBlock::DoRead()
{
    uint16_t numBlocks = GetBlocks(DEFAULT_BLOCK_NUM);

    if (numBlocks)
    {
    	m_LastX = m_Blocks[0].x;
    	m_LastY = m_Blocks[0].y;
    	printf("PixyBlock::DoRead(): numBlocks=%d x=%d y=%d\n", numBlocks, m_LastX, m_LastY);
    }
    else
    {
    	printf("PixyBlock::DoRead(): no blocks seen\n");
    }
}

void PixyBlock::Handle()
{
	m_Timestamp = Timer::GetFPGATimestamp();

	while (1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(30));

		if(!m_EnableRead)
		{
			m_I2C->Write(0x00, 0);
			continue;
		}

		DoRead();

		m_Count++;

		if ((Timer::GetFPGATimestamp() - m_Timestamp) > 1)
		{
			printf("PixyBlock::Handle():m_Count=%d\n", m_Count);
			m_Count = 0;
			m_Timestamp = Timer::GetFPGATimestamp();
		}
	}
}

PixyBlock *PixyBlock::GetInstance()
{
	if (m_Instance == NULL)
	{
		m_Instance = new PixyBlock();
		std::cout << "Made a pixy" << (int)m_Instance << std::endl;
	}
	return m_Instance;
}

uint16_t PixyBlock::GetBlocks(uint16_t maxBlocks)
{
	m_Blocks[0] = { 0 };
	uint8_t i;
	uint16_t w, checksum, sum;
	Block *block;
	bool skipStart = false;
	uint16_t blockCount;

	if (!skipStart) // When computer has not seen 0xaa55 (starting frame)
	{
		if (!GetStart())
			return 0;
	}
	else
	{
		skipStart = false;
	}

	for (blockCount = 0; blockCount < maxBlocks && blockCount < PIXY_MAXIMUM_ARRAYSIZE; )
	{
		checksum = GetWord();

		// We've reached the beginning of the next frame - checking for 0xaa55
		if (checksum == PIXY_START_WORD)
		{
			skipStart = true; // Starts this function
			m_BlockType = NORMAL_BLOCK;
			//Serial.println("skip");
			return blockCount;
		}
		// We've reached the beginning of the next frame - checking for 0xaa56
		else if (checksum == PIXY_START_WORD_CC)
		{
			skipStart = true;
			m_BlockType = CC_BLOCK;
			return blockCount;
		}
		else if (checksum == 0)
			return blockCount;

		//if (blockCount>blockArraySize)
		//resize();

		block = m_Blocks + blockCount;

		for (i = 0, sum = 0; i < sizeof(Block) / sizeof(uint16_t); i++)
		{
			// Skip - if not an CC block, no need to consider angle
			if (m_BlockType == NORMAL_BLOCK && i >= 5)
			{
				block->angle = 0;
				break;
			}
			w = GetWord();
			sum += w;
			*((uint16_t *)block + i) = w; // Converts block to integer value
		}

		if (checksum == sum)
			blockCount++;
		else
			printf("Pixy: cs error");

		w = GetWord(); // When this is start of the frame

		if (w == PIXY_START_WORD)
			m_BlockType = NORMAL_BLOCK;
		else if (w == PIXY_START_WORD_CC)
			m_BlockType = CC_BLOCK;
		else
			return blockCount;
	}
	return 0;
}

void PixyBlock::Print(Block *block)
{
	int i, j;
	char sig[6], d;
	bool flag;

	// Color code (CC)
	if (block->signature > PIXY_MAX_SIGNATURE)
	{
		// Convert signature number to an octal string
		// Assigns value to signature, x, y, width, height, and angle
		for (i = 12, j = 0, flag = false; i >= 0; i -= 3)
		{
			d = (block->signature >> i) & 0x07;
			if (d > 0 && !flag)
				flag = true;
			if (flag)
				sig[j++] = d + '0';
		}
		sig[j] = '\0';
		printf("CC block! sig: %s (%d decimal) x: %d y: %d width: %d height: %d angle %d\n",
				sig, block->signature, block->x, block->y,
				block->width, block->height, block->angle);
	}
	// Regular block - note, angle is always zero, so no need to print
	else
		printf("sig: %d x: %d y: %d width: %d height: %d\n",
				block->signature, block->x, block->y,
				block->width, block->height);
}

// Checks whether if it is start of the normal frame, CC frame, or the data is out of sync
bool PixyBlock::GetStart()
{
	uint16_t word;
	uint16_t lastWord;

	lastWord = 0xffff;

	while (true)
	{
		word = GetWord();
		if (word == 0 && lastWord == 0)
		{
			//delayMicroseconds(10);
			return false;
		}
		else if (word == PIXY_START_WORD && lastWord == PIXY_START_WORD)
		{
			m_BlockType = NORMAL_BLOCK;
			return true;
		}
		else if (word == PIXY_START_WORD_CC && lastWord == PIXY_START_WORD)
		{
			m_BlockType = CC_BLOCK;
			return true;
		}
		// When byte received was 0x55aa instead of other way around, the code syncs the byte
		else if (word == PIXY_START_WORDX)
		{
			//printf("Pixy: reorder");
			GetByte(); // resync
		}
		lastWord = word;
	}
}

uint16_t PixyBlock::GetWord()
{
   uint8_t buffer[2] = { 0, 0 };

   int bytes = 0;
   bytes += m_I2C->ReadOnly(1, &buffer[0]);
   bytes += m_I2C->ReadOnly(1, &buffer[1]);
   printf("Received word: 0x%x 0x%x\r\n", buffer[0], buffer[1]);
   return (buffer[1] << 8) | buffer[0];
}

uint8_t PixyBlock::GetByte()//gets a byte
{
   uint8_t buffer[1] = { 0 };

   int bytes = m_I2C->ReadOnly(1, buffer);
   std::cout << "Received byte: " << bytes << std::endl;

   return buffer[0];
}

int PixyBlock::GetX()
{
	return m_LastX;
}

int PixyBlock::GetY()
{
	return m_LastY;
}

void PixyBlock::SetRead(bool val)
{
	m_EnableRead = val;
}
