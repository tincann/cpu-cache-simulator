#include "precomp.h"
#include "cachecontroller.h"
#include "cache.h"

CacheController::CacheController()
{
	ram = new RAM;
}

int CacheController::READ(int * address)
{
	return ram->Read(address);
}

void CacheController::WRITE(int* address, int value)
{
	ram->Write(address, value);
}
