#pragma once
#include "Memory.h"


class RAM : public Memory {
public:
	CacheLine ReadCacheLine(int * address) override;
	void Write(int * address, CacheLine value) override;
};

class Cache : public Memory {
protected:
	glm::uint setmask = 0;
	glm::uint tagmask = 0;
	glm::uint setcount = 0;
	glm::uint slotcount = 0;
	Memory *decorates; // What is written to/read from on cache miss
	CacheLine** cache;

	
	//virtual int BestSlotToOverwrite() = 0; // in fully associative cache
	int BestSlotToOverwrite(uint address); // in N-way associative cache

	~Cache();
public:
	Cache(Memory * decorates, uint setcount, uint slotcount);

	CacheLine ReadCacheLine(int * address) override;
	void Write(int * address, CacheLine value) override;
};
