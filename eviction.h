#pragma once
class EvictionPolicy
{
public:
	EvictionPolicy(int setsize, int slotsize);
	virtual ~EvictionPolicy() {};
	virtual int BestSlotToOverwrite(int set) = 0;
	virtual void CachelineInserted(int set, int slot) = 0;
	//virtual void CachelineRemoved(int set, int slot) = 0;

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
