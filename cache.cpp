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

Cache::Cache(Memory * decorates, uint setcount, uint slotcount)
{
	this->decorates = decorates;
	this->setcount = setcount;
	this->slotcount = slotcount;
	this->setmask = (setcount - 1) << OFFSET;
	this->tagmask = ~(setmask | 0xF);

	cache = new CacheLine*[setcount];
	for (uint i = 0; i < setcount; i++) {
		cache[i] = new CacheLine[slotcount];
		for (uint j = 0; j < slotcount;j++)
		{
			cache[i][j] = { 0 };
		}
	}
}

int Cache::Read(int * address)
{
	auto addr = reinterpret_cast<uint>(address);
	auto set = (addr & setmask) >> OFFSET;
	auto tag = (addr & tagmask);
	auto slots = cache[set];

	for (int i = 0; i < slotcount; i++) {
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

//int L1Cache::Read(int * address)
//{
//	return Cache::Read(address);
//}
//
//int L1Cache::BestSlotToOverwrite()
//{
//	return 0;
//}
//
//int L1Cache::BestSlotToOverwrite(int address)
//{
//	return 0;
//}
