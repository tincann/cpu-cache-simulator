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

	int READ(int * address);
	void WRITE(int * address, int value);

	void PrintDebug(Surface* surface);
};
