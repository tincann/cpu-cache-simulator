#pragma once

#define L1SETS 1024
#define L1SLOTS 4
#define L2SETS 4096
#define L2SLOTS 8
#define L3SETS 16384
#define L3SLOTS 16

#define DIRTYMASK 0x1
#define VALIDMASK 0x2
#define OFFSETMASK 0x3 << 2

#define L1SETMASK (L1SETS - 1) << 4
#define L2SETMASK (L2SETS - 1) << 4
#define L3SETMASK (L3SETS - 1) << 4

// Represents any kind of memory (cache or RAM)
// The cache hierarchy is represented with a decorator pattern (L1 -> L2 -> L3 -> RAM)
class Memory {
public:
	virtual int Read(int* address) = 0;
	virtual void Write(int* address, int value) = 0;
};

class RAM : Memory {
public:
	int Read(int* address);
	void Write(int* address, int value);
};

struct CacheLine {
	unsigned int address;
	unsigned int data;
};

class Cache : Memory {
protected:
	Memory *decorates; // What is written to/read from on cache miss

	virtual int BestSlotToOverwrite() = 0; // in fully associative cache
	virtual int BestSlotToOverwrite(int address) = 0; // in N-way associative cache

	~Cache();
public:
	Cache(Memory *decorates);

	virtual int Read(int* address);
	virtual void Write(int* address, int value);
};

class L1Cache : Cache {
private:
	CacheLine cache[L1SETS][L1SLOTS];
};