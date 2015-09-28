#pragma once
#include "cache.h"

//#define EVICTION_POLICY StaticEviction
//#define EVICTION_POLICY RandomEviction
//#define EVICTION_POLICY LRUEviction
//#define EVICTION_POLICY MRUEviction
#define EVICTION_POLICY LFUEviction

// Amount of slots in L1/L2/L3 cache
#define L1SLOTS 4
#define L2SLOTS 8
#define L3SLOTS 16

#define RAMSLEEPTIME 0
#define L1SLEEPTIME 0
#define L2SLEEPTIME 0
#define L3SLEEPTIME 0

// Amount of sets in L1/L2/L3 cache
#define KB * 1024
#define MB * 1024 KB
#define L1SETS ((32 KB) / CACHELINELENGTH / L1SLOTS)
#define L2SETS ((256 KB) / CACHELINELENGTH / L2SLOTS)
#define L3SETS ((2 MB) / CACHELINELENGTH / L3SLOTS)

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

	// Read methods for all common types
	double READDOUBLE(int * address);
	long long READLONG(int * address);
	uint READUINT(int * address);
	int READINT(int * address);
	float READFLOAT(int * address);
	int READSHORT(int * address);
	char READCHAR(int * address);
	byte READBYTE(int * address);

	// Write overloads for all common types
	void WRITE(int * address, double value);
	void WRITE(int * address, long long value);
	void WRITE(int * address, unsigned int value);
	void WRITE(int * address, int value);
	void WRITE(int * address, float value);
	void WRITE(int * address, short value);
	void WRITE(int * address, char value);
	void WRITE(int * address, byte value);

	// Show debug info on the screen
	void PrintDebug(Surface* surface);
};
