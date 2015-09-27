#pragma once
#include "cache.h"

//#define EVICTION_POLICY StaticEviction
#define EVICTION_POLICY RandomEviction

#define L1SLOTS 4
#define L1SETS ((32 * 1024) / CACHELINELENGTH / L1SLOTS)
#define L2SLOTS 8
#define L2SETS ((256 * 1024) / CACHELINELENGTH / L2SLOTS)
#define L3SLOTS 16
#define L3SETS ((2 * 1024 * 1024) / CACHELINELENGTH / L3SLOTS)

class CacheController {
private:
	RAM * ram;
	Cache * l3cache;
	Cache * l2cache;
	Cache * l1cache;

	CacheController();

	CacheController(CacheController const&) = delete;
	void operator=(CacheController const&) = delete;
public:
	static CacheController& getInstance()
	{
		static CacheController instance;
		return instance;
	}

	double READDOUBLE(int * address);
	long long READLONG(int * address);
	uint READUINT(int * address);
	int READINT(int * address);
	float READFLOAT(int * address);
	int READSHORT(int * address);
	char READCHAR(int * address);
	byte READBYTE(int * address);

	void WRITE(int * address, double value);
	void WRITE(int * address, long long value);
	void WRITE(int * address, unsigned int value);
	void WRITE(int * address, int value);
	void WRITE(int * address, float value);
	void WRITE(int * address, short value);
	void WRITE(int * address, char value);
	void WRITE(int * address, byte value);

	void PrintDebug(Surface* surface);
};
