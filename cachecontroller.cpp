#include "precomp.h"
#include "cachecontroller.h"
#include "cache.h"

CacheController::CacheController()
{
	ram = new RAM;
	l1cache = new L1Cache(ram);
}

int CacheController::READ(int * address)
{
	return l1cache->Read(address);
}

void CacheController::WRITE(int * address, int value)
{
	ram->Write(address, value);
}
