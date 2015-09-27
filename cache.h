#pragma once
#include "precomp.h"
#include "eviction.h"

// Cache line length is in bytes
#define CACHELINELENGTH 64
// must be log2(CACHELINELENGTH / 4)
#define OFFSET 4
#define DIRTYMASK 0x1
#define VALIDMASK 0x2

// Full offset mask, includes first two bits
#define OFFSETMASK ((((int) pow(2.0, OFFSET) - 1) << 2) | DIRTYMASK | VALIDMASK)
// Offset mask for byte values
#define BYTEOFFSETMASK OFFSETMASK
// Offset mask for WORD values (shorts etc)
#define WORDOFFSETMASK (OFFSETMASK & ~(0x1))
// Offset mask for DWORD values
#define DWORDOFFSETMASK (OFFSETMASK & ~(0x3))
// Offset mask for QWORD values
#define QWORDOFFSETMASK (OFFSETMASK & ~(0x7))

#define IsDirty(addr) (addr & DIRTYMASK)
#define IsValid(addr) (addr & VALIDMASK)

inline uint GetByteOffset(int * address)
{
	auto addr = reinterpret_cast<uint>(address);
	return addr & BYTEOFFSETMASK;
}

inline uint GetWORDOffset(int * address)
{
	auto addr = reinterpret_cast<uint>(address);
	return (addr & WORDOFFSETMASK) >> 1;
}

inline uint GetDWORDOffset(int * address)
{
	auto addr = reinterpret_cast<uint>(address);
	return (addr & DWORDOFFSETMASK) >> 2;
}

inline uint GetQWORDOffset(int * address)
{
	auto addr = reinterpret_cast<uint>(address);
	return (addr & QWORDOFFSETMASK) >> 3;
}

struct CacheLine {
	unsigned int address;
	union {
		double d_data[CACHELINELENGTH / sizeof(double)];
		long long l_data[CACHELINELENGTH / sizeof(long long)];
		unsigned int ui_data[CACHELINELENGTH / sizeof(unsigned int)];
		int i_data[CACHELINELENGTH / sizeof(int)];
		float f_data[CACHELINELENGTH / sizeof(float)];
		short s_data[CACHELINELENGTH / sizeof(short)];
		char c_data[CACHELINELENGTH / sizeof(char)];
		byte b_data[CACHELINELENGTH / sizeof(byte)];
	};
};

// Represents any kind of memory (cache or RAM)
// The cache hierarchy is represented with a decorator pattern (L1 -> L2 -> L3 -> RAM)
class Memory {
public:
	virtual ~Memory()
	{
	}

	double ReadDouble(int * address);
	void WriteDouble(int * address, double value);
	long long ReadLong(int * address);
	void WriteLong(int * address, long long value);
	uint ReadUInt(int * address);
	void WriteUInt(int * address, uint value);
	int ReadInt(int * address);
	void WriteInt(int * address, int value);
	float ReadFloat(int * address);
	void WriteFloat(int * address, float value);
	int ReadShort(int * address);
	void WriteShort(int * address, short value);
	char ReadChar(int * address);
	void WriteChar(int * address, char value);
	byte ReadByte(int * address);
	void WriteByte(int * address, byte value);


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
	uint setmask = 0;
	uint tagmask = 0;
	uint setcount = 0;
	uint slotcount = 0;
	Memory *decorates; // What is written to/read from on cache miss
	CacheLine** cache;
	EvictionPolicy *eviction_policy;

	~Cache();
public:
	Cache(Memory * decorates, uint setcount, uint slotcount, EvictionPolicy *eviction_policy);

	CacheLine ReadCacheLine(int * address) override;
	void Write(int * address, CacheLine value) override;
};
