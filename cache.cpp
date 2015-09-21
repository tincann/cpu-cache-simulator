#include "cache.h"
#include "cachecontroller.h"

int RAM::Read(int * address)
{
	return READ(address);
}

void RAM::Write(int * address, int value)
{
	WRITE(address, value);
}
