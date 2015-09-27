#pragma once
#include "precomp.h"

// Cache line length is in bytes
#define CACHELINELENGTH 64
// must be log2(CACHELINELENGTH / 4)
#define OFFSET 4
#define DIRTYMASK 0x1
#define VALIDMASK 0x2

// Full offset mask, includes first two bits
#define OFFSETMASK ((((int) pow(2.0, OFFSET) - 1) << 2) | DIRTYMASK | VALIDMASK)
// Offset mask for DWORD values
#define DWORDOFFSETMASK (OFFSETMASK & ~(0x3))
// Offset mask for QWORD values
#define DWORDOFFSETMASK (OFFSETMASK & ~(0x7))

#define IsDirty(addr) (addr & DIRTYMASK)
#define IsValid(addr) (addr & VALIDMASK)

inline uint GetDWORDOffset(int * address)
{
	auto a = OFFSETMASK;
	auto b = DWORDOFFSETMASK;
	auto addr = reinterpret_cast<uint>(address);
	return (addr & DWORDOFFSETMASK) >> 2;
}

struct CacheLine {
	unsigned int address;
	union {
		double d_data[CACHELINELENGTH / 8];
		long l_data[CACHELINELENGTH / 8];
		unsigned int ui_data[CACHELINELENGTH / 4];
		int i_data[CACHELINELENGTH / 4];
		float f_data[CACHELINELENGTH / 4];
		char c_data[CACHELINELENGTH];
		byte b_data[CACHELINELENGTH];
	};
};

// Represents any kind of memory (cache or RAM)
// The cache hierarchy is represented with a decorator pattern (L1 -> L2 -> L3 -> RAM)
class Memory {
public:
	virtual ~Memory()
	{
	}

	int ReadInt(int * address);
	void WriteInt(int * address, int value);
	float ReadFloat(int * address);
	void WriteFloat(int * address, float value);

	virtual CacheLine ReadCacheLine(int * address) = 0;
	virtual void Write(int * address, CacheLine value) = 0;
};

class RAM : public Memory {
public:
	CacheLine ReadCacheLine(int * address) override;
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

	CacheLine ReadCacheLine(int * address) override;
	void Write(int * address, CacheLine value) override;
};
