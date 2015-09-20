#pragma once

// Represents any kind of memory (cache or RAM)
// The cache hierarchy is represented with a decorator pattern (L1 -> L2 -> L3 -> RAM)
class Memory {
public:
	virtual int Read(int* address) = 0;
	virtual void Write(int* address, int value) = 0;
	virtual ~Memory() = 0;
};

class RAM : public Memory {
	int Read(int* address);
	void Write(int* address, int value);
};

struct CacheLine {
	unsigned int tag;
	unsigned int data;
};

class Cache : public Memory {
protected:
	Memory *decorates; // What is written to/read from on cache miss

	CacheLine cache [10];
	virtual int BestSlotToOverwrite() = 0; // in fully associative cache
	virtual int BestSlotToOverwrite(int address) = 0; // in N-way associative cache

public:
	virtual int Read(int* address) = 0;
	virtual void Write(int* address, int value) = 0;

	//virtual Cache(Memory parent); 
};