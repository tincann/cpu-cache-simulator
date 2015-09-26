#include "precomp.h"
#include "cachecontroller.h"
#include "cache.h"


CacheController::CacheController()
{
	ram = new RAM;
	l3cache = new Cache(ram, L3SETS, L3SLOTS);
	l2cache = new Cache(l3cache, L2SETS, L2SLOTS);
	l1cache = new Cache(l2cache, L1SETS, L1SLOTS);
}

int CacheController::READ(int * address)
{
	return l1cache->Read(address);
}

void CacheController::WRITE(int * address, int value)
{
	l1cache->Write(address, value);
}
