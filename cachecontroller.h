#pragma once
#include "cache.h"

#define L1SETS 1024
#define L1SLOTS 4
#define L2SETS 4096
#define L2SLOTS 8
#define L3SETS 16384
#define L3SLOTS 16

class CacheController {
private:
	RAM * ram;
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
};
