#pragma once
#include "precomp.h"

// Cache line length is in bytes
#define CACHELINELENGTH 64
// must be log2(CACHELINELENGTH / 4)
#define OFFSET 4
#define DIRTYMASK 0x1
#define VALIDMASK 0x2
#define OFFSETMASK ((int) pow(2.0, OFFSET) - 1) << 2

#define GetOffset(addr) (addr & OFFSETMASK >> 2)
#define IsDirty(addr) (addr & DIRTYMASK)
#define IsValid(addr) (addr & VALIDMASK)

struct CacheLine {
	unsigned int address;
	unsigned int data[CACHELINELENGTH / 4];
};

// Represents any kind of memory (cache or RAM)
// The cache hierarchy is represented with a decorator pattern (L1 -> L2 -> L3 -> RAM)
class Memory {
public:
	virtual ~Memory()
	{
	}

	virtual int Read(int * address) = 0;
	virtual void Write(int * address, int value) = 0;
	virtual void Write(int * address, CacheLine value) = 0;
};

class RAM : public Memory {
public:
	int Read(int * address) override;
	void Write(int * address, int value) override;
	void Write(int * address, CacheLine value) override;
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
	int BestSlotToOverwrite(uint address); // in N-way associative cache

	~Cache();
public:
	Cache(Memory * decorates, uint setcount, uint slotcount);

	int Read(int * address) override;
	void Write(int * address, int value) override;
	void Write(int * address, CacheLine value) override;
};
