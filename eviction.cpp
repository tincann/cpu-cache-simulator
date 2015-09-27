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
	uint min = ~0;
	uint minIndex = 0;

	for (auto i = 0; i < slotsize;i++)
	{
		auto age = cacheline_ages[set][i];

		// Age has wrapped around, get rid of them from the cache ASAP
		if (age > current_age)
			return i;

		if (age < min)
		{
			min = age;
			minIndex = i;
		}
	}

	return minIndex;
}

void LRUEviction::CachelineInserted(int set, int slot)
{
	cacheline_ages[set][slot] = ++current_age;
}