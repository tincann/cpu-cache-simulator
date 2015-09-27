#include "precomp.h"
#include "cachecontroller.h"
#include "cache.h"

CacheController::CacheController()
{

	ram = new RAM;
	l3cache = new Cache(ram, L3SETS, L3SLOTS, new EVICTION_POLICY(L3SETS, L3SLOTS));
	l2cache = new Cache(l3cache, L2SETS, L2SLOTS, new EVICTION_POLICY(L2SETS, L2SLOTS));
	l1cache = new Cache(l2cache, L1SETS, L1SLOTS, new EVICTION_POLICY(L1SETS, L1SLOTS));
}

double CacheController::READDOUBLE(int * address)
{
	return l1cache->ReadDouble(address);
}

long long CacheController::READLONG(int * address)
{
	return l1cache->ReadLong(address);
}

uint CacheController::READUINT(int * address)
{
	return l1cache->ReadUInt(address);
}

int CacheController::READINT(int * address)
{
	return l1cache->ReadInt(address);
}

float CacheController::READFLOAT(int * address)
{
	return l1cache->ReadFloat(address);
}

int CacheController::READSHORT(int * address)
{
	return l1cache->ReadShort(address);
}

char CacheController::READCHAR(int * address)
{
	return l1cache->ReadChar(address);
}

byte CacheController::READBYTE(int * address)
{
	return l1cache->ReadByte(address);
}

void CacheController::WRITE(int * address, double value)
{
	l1cache->WriteDouble(address, value);
}

void CacheController::WRITE(int * address, long long value)
{
	l1cache->WriteLong(address, value);
}

void CacheController::WRITE(int * address, unsigned int value)
{
	l1cache->WriteUInt(address, value);
}

void CacheController::WRITE(int * address, int value)
{
	l1cache->WriteInt(address, value);
}

void CacheController::WRITE(int * address, float value)
{
	l1cache->WriteFloat(address, value);
}

void CacheController::WRITE(int * address, short value)
{
	l1cache->WriteShort(address, value);
}

void CacheController::WRITE(int * address, char value)
{
	l1cache->WriteChar(address, value);
}

void CacheController::WRITE(int * address, byte value)
{
	l1cache->WriteByte(address, value);
}

void CacheController::PrintDebug(Surface* surface)
{
	char t[256];
	sprintf(t, "L1 hits/misses/hitrate: %d / %d / %06.3f", l1cache->hit, l1cache->miss, l1cache->hit / static_cast<float>(l1cache->miss));
	surface->Print(t, 2, 2, 0xffffff);
	sprintf(t, "L2 hits/misses/hitrate: %d / %d / %06.3f", l2cache->hit, l2cache->miss, l2cache->hit / static_cast<float>(l2cache->miss));
	surface->Print(t, 2, 12, 0xffffff);
	sprintf(t, "L3 hits/misses/hitrate: %d / %d / %06.3f", l3cache->hit, l3cache->miss, l3cache->hit / static_cast<float>(l3cache->miss));
	surface->Print(t, 2, 22, 0xffffff);
	uint th, tm;
	sprintf(t, "Total hits/misses/hitrate: %d / %d / %06.3f %%", th = l1cache->hit + l2cache->hit + l3cache->hit, tm = l1cache->miss + l2cache->miss + l3cache->miss, th / static_cast<float>(tm));
	surface->Print(t, 2, 32, 0xff0000);
}