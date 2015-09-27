#include "precomp.h"
#include "cachecontroller.h"
#include "cache.h"


CacheController::CacheController()
{
	ram = new RAM;
	l3cache = new Cache(ram, L3SETS, L3SLOTS, new StaticEviction(L3SETS, L3SLOTS));
	l2cache = new Cache(l3cache, L2SETS, L2SLOTS, new StaticEviction(L2SETS, L2SLOTS));
	l1cache = new Cache(l2cache, L1SETS, L1SLOTS, new StaticEviction(L1SETS, L1SLOTS));
}

int CacheController::READ(int * address)
{
	return l1cache->ReadInt(address);
}

void CacheController::WRITE(int * address, int value)
{
	l1cache->WriteInt(address, value);
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