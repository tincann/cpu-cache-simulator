#include "precomp.h"
#include "cachecontroller.h"
#include "cache.h"


CacheController::CacheController()
{
	ram = new RAM;
	l1cache = new Cache(ram, L1SETS, L1SLOTS);
}

int CacheController::READ(int * address)
{
	return l1cache->Read(address);
}

void CacheController::WRITE(int * address, int value)
{
	ram->Write(address, value);
}
