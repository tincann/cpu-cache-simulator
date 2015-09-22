#include "precomp.h"
#include "cache.h"

int RAM::Read(int * address)
{
	return ReadFromRAM(address);
}

void RAM::Write(int * address, int value)
{
	WriteToRAM(address, value);
}

Cache::~Cache()
{
	delete decorates;
}

Cache::Cache(Memory * decorates)
{
	this->decorates = decorates;
}
