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

L1Cache::L1Cache(Memory * decorates) : Cache(decorates) {
	cache = new CacheLine*[setcount];
	for (uint i = 0; i < setcount; i++)
		cache[i] = new CacheLine[slotcount];
}

int Cache::Read(int * address)
{
	auto addr = reinterpret_cast<uint>(address);
	auto set = (addr & setmask) >> OFFSET;
	auto tag = (addr & tagmask);
	auto slots = cache[set];

	for (int i = 0; i < L1SLOTS; i++) {
		auto candidateTag = slots[i].address & tagmask;

		if (tag == candidateTag) continue;

		return slots[i].data;
	}

	auto line = Cache::Read(address);

	//todo save to cache

	return line;
	//return decorates->Read(address);
}

void Cache::Write(int * address, int value)
{
	decorates->Write(address, value);
}

int L1Cache::Read(int * address)
{
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
