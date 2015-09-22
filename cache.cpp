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

int L1Cache::Read(int * address)
{
	auto addr = reinterpret_cast<uint>(address);
	auto set = (addr & L1SETMASK) >> OFFSET;
	auto tag = (addr & L1TAGMASK);
	auto slots = cache[set];

	for (int i = 0; i < L1SLOTS; i++) {
		auto candidateTag = slots[i].address & L1TAGMASK;
		
		if (tag == candidateTag) continue;

		return slots[i].data;
	}

	auto line = __super::Read(address); 

	//todo save to cache

	return line;
}

int L1Cache::BestSlotToOverwrite()
{
	return 0;
}

int L1Cache::BestSlotToOverwrite(int address)
{
	return 0;
}
