#pragma once
#include "memory.h"
#include "eviction.h"


class RAM : public Memory {
private:
	int sleeptime;
public:
	RAM(int sleeptime) : sleeptime(sleeptime) {};
	CacheLine ReadCacheLine(int * address) override;
	void Write(int * address, CacheLine value) override;
};

class Cache : public Memory {
protected:
	uint setmask = 0;
	uint tagmask = 0;
	uint setcount = 0;
	uint slotcount = 0;
	Memory *decorates; // What is written to/read from on cache miss
	CacheLine** cache;
	EvictionPolicy *eviction_policy;

	int sleeptime = 0;

	~Cache();
public:
	// Keep track of hits and misses
	uint hit = 0;
	uint miss = 0;

	Cache(Memory * decorates, uint setcount, uint slotcount, EvictionPolicy *eviction_policy, int sleeptime);

	CacheLine ReadCacheLine(int * address) override;
	void Write(int * address, CacheLine value) override;
};
