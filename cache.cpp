#include "precomp.h"
#include "cache.h"

float Memory::ReadFloat(int* address)
{
	union fp_bit_twiddler {
		float f;
		int i;
	} twiddler;

	twiddler.i = Read(address);

	return twiddler.f;
}

void Memory::WriteFloat(int* address, float value)
{
	union fp_bit_twiddler {
		float f;
		int i;
	} twiddler;

	twiddler.f = value;
	Write(address, twiddler.i);
}

int RAM::Read(int * address)
{
	return ReadFromRAM(address);
}

// Receive one cache line from RAM
CacheLine RAM::ReadCacheLine(int* address)
{
	CacheLine cacheline;
	auto addr = reinterpret_cast<uint>(address);
	auto startaddr = addr & ~(OFFSETMASK | DIRTYMASK | VALIDMASK);

	cacheline.address = startaddr | VALIDMASK;

	for (uint i = 0; i < CACHELINELENGTH / 4; i++)
	{
		cacheline.ui_data[i] = *reinterpret_cast<int *>(startaddr + i * 4);
	}

	return cacheline;
}

void RAM::Write(int * address, int value)
{
	auto addr = reinterpret_cast<uint>(address);
	addr = addr & ~(DIRTYMASK | VALIDMASK);

	WriteToRAM(reinterpret_cast<int*>(addr), value);
}

// write the contents of one cache line to RAM
void RAM::Write(int* address, CacheLine value)
{
	auto addr = reinterpret_cast<uint>(address);
	auto startaddr = addr & ~(OFFSETMASK | DIRTYMASK | VALIDMASK);

	for (uint i = 0; i < CACHELINELENGTH / 4; i++)
	{
		auto writeaddr = reinterpret_cast<int *>(startaddr + i * 4);
		*writeaddr = value.ui_data[i];
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
	this->setmask = (setcount - 1) << OFFSET << 2;
	this->tagmask = ~(setmask | OFFSETMASK | 0x3);

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
	auto offset = GetOffset(addr);
	auto line = ReadCacheLine(address);

	return line.i_data[offset];
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

		return slots[i];
	}

	auto line = decorates->ReadCacheLine(address);

	Write(address, line);

	return line;
}

void Cache::Write(int * address, int value)
{
	auto addr = reinterpret_cast<uint>(address);
	auto offset = GetOffset(addr);
	auto line = ReadCacheLine(address);

	line.address |= DIRTYMASK;
	line.i_data[offset] = value;

	Write(address, line);
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

		return;
	}

	// Look for invalid entries
	for (uint i = 0; i < slotcount; i++) {
		auto candidateAddr = slots[i].address;

		if (IsValid(candidateAddr)) continue;

		slots[i] = value;

		return;
	}

	auto overwrite = BestSlotToOverwrite(addr);
	auto overwriteAddr = slots[overwrite].address;

	// Write cacheline to RAM
	if (IsDirty(overwriteAddr))
		decorates->Write(reinterpret_cast<int *>(overwriteAddr), slots[overwrite]);

	slots[overwrite] = value;
}