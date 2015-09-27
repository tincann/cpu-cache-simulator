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
int StaticEviction::BestSlotToOverwrite(int set, int cacheline)
{
	return 0;
}

//
//int RandomEviction::BestSlotToOverwrite(uint address)
//{
//	return 0;
//}