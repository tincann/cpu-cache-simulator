#pragma once

// Decides which slot gets evicted
class EvictionPolicy
{
public:
	EvictionPolicy(int setsize, int slotsize);
	virtual ~EvictionPolicy() {};
	virtual int BestSlotToOverwrite(int set) = 0;
	virtual void CachelineInserted(int set, int slot) = 0;
	virtual void CachelineRead(int set, int slot) {};
	virtual void CachelineRemoved(int set, int slot) {};

protected:
	int setsize;
	int slotsize;
};

class StaticEviction : public EvictionPolicy
{
public:

	explicit StaticEviction(int setsize, int slotsize)
		: EvictionPolicy(setsize, slotsize)
	{
	}

	int BestSlotToOverwrite(int set) override;
	void CachelineInserted(int set, int slot) override {};
	//void CachelineRemoved(int set, int slot) override {};
};

class RandomEviction : public EvictionPolicy
{
public:
	RandomEviction(int setsize, int slotsize)
		: EvictionPolicy(setsize, slotsize)
	{
	}

	int BestSlotToOverwrite(int set) override;
	void CachelineInserted(int set, int slot) override {};
};

class LRUEviction : public EvictionPolicy
{
public:
	LRUEviction(int setsize, int slotsize)
		: EvictionPolicy(setsize, slotsize)
	{
		cacheline_ages = new int*[setsize];
		for (uint i = 0; i < setsize; i++) {
			cacheline_ages[i] = new int[slotsize];
			for (uint j = 0; j < slotsize; j++)
			{
				cacheline_ages[i][j] = { 0 };
			}
		}
	}

	int BestSlotToOverwrite(int set) override;
	void CachelineInserted(int set, int slot) override;

private:
	int** cacheline_ages;
	uint current_age = 0;
};

class MRUEviction : public EvictionPolicy
{
public:
	MRUEviction(int setsize, int slotsize)
		: EvictionPolicy(setsize, slotsize)
	{
		cacheline_ages = new int*[setsize];
		for (uint i = 0; i < setsize; i++) {
			cacheline_ages[i] = new int[slotsize];
			for (uint j = 0; j < slotsize; j++)
			{
				cacheline_ages[i][j] = { 0 };
			}
		}
	}

	int BestSlotToOverwrite(int set) override;
	void CachelineInserted(int set, int slot) override;

private:
	int** cacheline_ages;
	uint current_age = 0;
};

// Least Often Used
class LFUEviction : public EvictionPolicy
{
public:
	LFUEviction(int setsize, int slotsize)
		: EvictionPolicy(setsize, slotsize)
	{
		cacheline_freqs = new int*[setsize];
		for (uint i = 0; i < setsize; i++) {
			cacheline_freqs[i] = new int[slotsize];
			for (uint j = 0; j < slotsize; j++)
			{
				cacheline_freqs[i][j] = { 0 };
			}
		}
	}

	int BestSlotToOverwrite(int set) override;
	void CachelineInserted(int set, int slot) override;
	void CachelineRead(int set, int slot) override;
	void CachelineRemoved(int set, int slot) override;
private:
	int** cacheline_freqs;
};