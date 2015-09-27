#include "precomp.h"
#include "eviction.h"


EvictionPolicy::EvictionPolicy(int setsize, int slotsize)
{
	this->setsize = setsize;
	this->slotsize = slotsize;
}

////////////
///Static///
////////////
int StaticEviction::BestSlotToOverwrite(int set)
{
	return 0;
}

////////////
///Random///
////////////
int RandomEviction::BestSlotToOverwrite(int set)
{
	return rand() % static_cast<int>(slotsize);
}

int LRUEviction::BestSlotToOverwrite(int set)
{
	return 0;
}

void LRUEviction::CachelineInserted(int set, int slot)
{}