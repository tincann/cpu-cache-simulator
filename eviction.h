#pragma once
class EvictionPolicy
{
public:
	EvictionPolicy(int setsize, int slotsize);
	virtual ~EvictionPolicy() {};
protected:
	int setsize;
	int slotsize;

	virtual int BestSlotToOverwrite(int set, int cacheline) = 0;
	virtual void CachelineInserted(int set, int cacheline) = 0;
};

class StaticEviction : public EvictionPolicy
{
public:

	explicit StaticEviction(int setsize, int slotsize)
		: EvictionPolicy(setsize, slotsize)
	{
	}

	int BestSlotToOverwrite(int set, int cacheline) override;
	void CachelineInserted(int set, int cacheline) override {};
};
//
//class RandomEviction : public EvictionPolicy
//{
//public:
//	int BestSlotToOverwrite(uint address) override;
//};
