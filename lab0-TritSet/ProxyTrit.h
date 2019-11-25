#pragma once
#include "Trit.h"
#include "TritSet.h"
#include "CalcArrSize.h"
#include <algorithm>

typedef unsigned uint;

class ProxyTrit
{
	friend struct TritSet;
	size_t index_; // index of trit	

	explicit ProxyTrit(size_t size);
	ProxyTrit(ProxyTrit const& set);
	ProxyTrit& operator=(ProxyTrit const& set);
	size_t capacity() const { return calcArrSize(capacity_)-1; }
	size_t cardianlity(Trit value) const;
	std::unordered_map< Trit, int, std::hash<int> > cardinality() const;
	size_t length() const { return ind_last_notional_ + 1; }

	//drop TritSet size_ to start value or to value enough to store set trit.
	void shrink() { resizeTrit(std::max(startArraySize_, ind_last_notional_)); }

	void trim(size_t lastIndex) { resizeTrit(lastIndex); }
	void setRand();

	Trit getValue(size_t const index) const;
	void copyTrit(const ProxyTrit & set);
	void updCountTF(size_t &curVal);
	void resizeTrit(size_t capacity);
	void calcLastNotional(const size_t & tritsIndex);
	void setValue(Trit const value, size_t& const index);

	size_t capacity_;
	uint* tritSet_;

	size_t const startArraySize_;

	size_t countTrue_; // how much True val in array
	size_t countFalse_; // how much False val in array
	size_t ind_last_notional_; // index of last non-unknown value

public:
	void operator=(Trit value);
	bool operator==(ProxyTrit const& value) const;
	bool operator==(Trit value) const;
	bool operator!=(ProxyTrit const& value) const;
	ProxyTrit operator|(ProxyTrit const& A) const;
	ProxyTrit operator&(ProxyTrit const& A) const;
	ProxyTrit operator~() const;
	~ProxyTrit() { delete[] tritSet_; }
};

void calcTritPalce(size_t & arrayElem, size_t & posInUint, uint & mask, size_t const index);
