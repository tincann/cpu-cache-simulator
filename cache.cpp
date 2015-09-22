#include "precomp.h"
#include "cache.h"

int RAM::Read(int * address)
{
	return ReadFromRAM((int*)address);
}

void RAM::Write(int * address, int value)
{
	WriteToRAM((int*)address, value);
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

int L1Cache::Read(int * address)
{
	uint addr = (uint)address;
	uint set = (addr & L1SETMASK) >> OFFSET;
	auto slots = cache[set];


	return 0;
}

int L1Cache::BestSlotToOverwrite()
{
	return 0;
}

int L1Cache::BestSlotToOverwrite(int address)
{
	return 0;
}
