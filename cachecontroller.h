#pragma once
#include "cache.h"

class CacheController {
private:
	RAM * ram;
	L1Cache * l1cache;

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
