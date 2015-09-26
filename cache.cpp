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

// write the contents of one cache line to RAM
void RAM::Write(int* address, CacheLine value)
{
	auto addr = reinterpret_cast<uint>(address);
	auto startaddr = addr & ~(OFFSETMASK | DIRTYMASK | VALIDMASK);

	for (uint i = 0; i < CACHELINELENGTH; i++)
	{
		auto writeaddr = reinterpret_cast<int *>(startaddr + i * 4);
		*writeaddr = value.data[i];
	}
}

int Cache::BestSlotToOverwrite(uint address)
{
	return 1;
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
	auto tag = addr & tagmask;
	auto slots = cache[set];

	for (uint i = 0; i < slotcount; i++) {
		auto candidateTag = slots[i].address;

		if (!(candidateTag & VALIDMASK)) continue; // cache line is invalid
		if (tag != (candidateTag & tagmask)) continue; // tag doesn't match

		// get int at offset
		auto offset = addr & OFFSETMASK;
		return slots[i].data[offset];
	}

	auto line = decorates->Read(address);

	Write(address, line);

	return line;
}

void Cache::Write(int * address, int value)
{
	auto addr = reinterpret_cast<uint>(address);
	auto set = (addr & setmask) >> OFFSET;
	auto tag = addr & tagmask;
	auto slots = cache[set];
	auto offset = addr & OFFSETMASK;

	// Check if the literal address exists in the memory
	for (uint i = 0; i < slotcount; i++) {
		auto candidateTag = slots[i].address;

		if (!(candidateTag & VALIDMASK)) continue; // invalid cache 
		if (tag != (candidateTag & tagmask)) continue; // tag doesn't match

		slots[i].data[offset] = value;
		slots[i].address |= DIRTYMASK;

		return;
	}

	// Look for invalid entries
	for (uint i = 0; i < slotcount; i++) {
		auto candidateTag = slots[i].address;

		if (candidateTag & VALIDMASK) continue;

		slots[i].data[offset] = value;
		slots[i].address = addr | VALIDMASK | DIRTYMASK;

		return;
	}

	auto overwrite = BestSlotToOverwrite(addr);
	auto overwriteAddr = slots[overwrite].address;

	// Write cacheline to RAM
	if (overwriteAddr & DIRTYMASK) 
		decorates->Write(reinterpret_cast<int *>(overwriteAddr), slots[overwrite]);

	slots[overwrite].address = addr | VALIDMASK | DIRTYMASK;
	slots[overwrite].data[offset] = value;
}

void Cache::Write(int* address, CacheLine value)
{
}