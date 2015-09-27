#include "precomp.h"
#include "cache.h"
#include "eviction.h"

// Receive one cache line from RAM
CacheLine RAM::ReadCacheLine(int* address)
{
	CacheLine cacheline;
	auto addr = reinterpret_cast<uint>(address);
	auto startaddr = addr & ~OFFSETMASK;

	cacheline.address = startaddr | VALIDMASK;

	for (uint i = 0; i < CACHELINELENGTH / 4; i++)
	{
		cacheline.ui_data[i] = *reinterpret_cast<int *>(startaddr + i * 4);
	}

	return cacheline;
}

// write the contents of one cache line to RAM
void RAM::Write(int* address, CacheLine value)
{
	auto addr = reinterpret_cast<uint>(address);
	auto startaddr = addr & ~(OFFSETMASK);

	for (uint i = 0; i < CACHELINELENGTH / 4; i++)
	{
		auto writeaddr = reinterpret_cast<int *>(startaddr + i * 4);
		*writeaddr = value.ui_data[i];
	}
}

Cache::~Cache()
{
	delete decorates;
}

Cache::Cache(Memory * decorates, uint setcount, uint slotcount, EvictionPolicy *eviction_policy)
{
	this->decorates = decorates;
	this->setcount = setcount;
	this->slotcount = slotcount;
	this->setmask = (setcount - 1) << OFFSET << 2;
	this->tagmask = ~(setmask | OFFSETMASK);
	this->eviction_policy = eviction_policy;

	cache = new CacheLine*[setcount];
	for (uint i = 0; i < setcount; i++) {
		cache[i] = new CacheLine[slotcount];
		for (uint j = 0; j < slotcount;j++)
		{
			cache[i][j] = { 0 };
		}
	}
}

CacheLine Cache::ReadCacheLine(int* address)
{
	auto addr = reinterpret_cast<uint>(address);
	auto set = (addr & setmask) >> OFFSET >> 2;
	auto tag = addr & tagmask;
	auto slots = cache[set];

	for (uint i = 0; i < slotcount; i++) {
		auto candidateAddr = slots[i].address;
		if (!IsValid(candidateAddr)) continue; // cache line is invalid
		if (tag != (candidateAddr & tagmask)) continue; // tag doesn't match
		hit++;
		return slots[i];
	}

	auto line = decorates->ReadCacheLine(address);

	Write(address, line);

	miss++;
	return line;
}

// untested, write a cache line to the cache
void Cache::Write(int* address, CacheLine value)
{
	auto addr = reinterpret_cast<uint>(address);
	auto set = (addr & setmask) >> OFFSET >> 2;
	auto tag = addr & tagmask;
	auto slots = cache[set];

	// Check if the literal address exists in the memory
	for (uint i = 0; i < slotcount; i++) {
		auto candidateAddr = slots[i].address;

		if (!IsValid(candidateAddr)) continue; // invalid cache 
		if (tag != (candidateAddr & tagmask)) continue; // tag doesn't match

		slots[i] = value;

		eviction_policy->CachelineInserted(set, i);

		return;
	}

	// Look for invalid entries
	for (uint i = 0; i < slotcount; i++) {
		auto candidateAddr = slots[i].address;

		if (IsValid(candidateAddr)) continue;

		slots[i] = value;

		eviction_policy->CachelineInserted(set, i);

		return;
	}

	auto overwrite = eviction_policy->BestSlotToOverwrite(set);
	auto overwriteAddr = slots[overwrite].address;

	// Write cacheline to RAM
	if (IsDirty(overwriteAddr)) {
		decorates->Write(reinterpret_cast<int *>(overwriteAddr), slots[overwrite]);
	}

	slots[overwrite] = value;

	eviction_policy->CachelineInserted(set, overwrite);
}