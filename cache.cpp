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

int Cache::Read(int * address)
{
	return decorates->Read(address);
}

void Cache::Write(int * address, int value)
{
	decorates->Write(address, value);
}

int L1Cache::Read(int* address)
{
	return 0;
}
