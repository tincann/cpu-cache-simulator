#pragma once
#include "precomp.h"

// Cache line length is in bytes
#define CACHELINELENGTH 64
// must be log2(CACHELINELENGTH)
#define OFFSET 4
#define DIRTYMASK 0x1
#define VALIDMASK 0x2
#define OFFSETMASK (((int) pow(2.0, OFFSET) - 1) & ~(DIRTYMASK | VALIDMASK))

#define L1SETMASK (L1SETS - 1) << OFFSET
#define L2SETMASK (L2SETS - 1) << OFFSET
#define L3SETMASK (L3SETS - 1) << OFFSET

#define L1TAGMASK ~(L1SETMASK | 0xC)
#define L2TAGMASK ~(L2SETMASK | 0xC)
#define L3TAGMASK ~(L3SETMASK | 0xC)

// Represents any kind of memory (cache or RAM)
// The cache hierarchy is represented with a decorator pattern (L1 -> L2 -> L3 -> RAM)
class Memory {
public:
	virtual ~Memory()
	{
	}

	virtual int Read(int * address) = 0;
	virtual void Write(int * address, int value) = 0;
};

class RAM : public Memory {
public:
	int Read(int * address) override;
	void Write(int * address, int value) override;
};

struct CacheLine {
	unsigned int address;
	unsigned int data[CACHELINELENGTH / 4];
};

class Cache : public Memory {
protected:
	glm::uint setmask = 0;
	glm::uint tagmask = 0;
	glm::uint setcount = 0;
	glm::uint slotcount = 0;
	Memory *decorates; // What is written to/read from on cache miss
	CacheLine** cache;

	
	//virtual int BestSlotToOverwrite() = 0; // in fully associative cache
	//virtual int BestSlotToOverwrite(int address) = 0; // in N-way associative cache

	~Cache();
public:
	Cache(Memory * decorates, uint setcount, uint slotcount);

	virtual int Read(int * address) override;
	virtual void Write(int * address, int value) override;
};
