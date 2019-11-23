#pragma once
#include <unordered_map>
#include "Trit.h"

typedef unsigned uint;

class ProxyTrit
{
public:
	ProxyTrit(uint& tritSet, size_t index);
	ProxyTrit(TritSet const& tritSet, size_t index);

	void operator=(Trit value);
	void operator=(uint tritArray);
	bool operator==(Trit value);
	bool operator!=(Trit value);
	uint operator|(ProxyTrit const& A) const;
	uint operator&(ProxyTrit const& A) const;
	uint operator~() const;

private:
	void setValue(Trit const value, size_t const index);
	Trit getValue(size_t const index);

	uint* tritSet_;
	size_t index_; // index of trit	
	size_t lastAvaliableIndex_; // contain avaliable array index
};

class TritSet
{
private:

	ProxyTrit set_;
	size_t startArraySize_;
	size_t arraySize_; // size of uint array
	uint* tritSet_; // pointer on array of uint
	size_t ind_last_notional_; // last non-Unknown Trit
	size_t countTrue_;
	size_t countFalse_;

public:
	TritSet(size_t capacity); //ready
	TritSet(TritSet const& set);

	ProxyTrit operator[](size_t index); //ready
	TritSet& operator=(TritSet const& set); //ready
	ProxyTrit operator[](size_t index) const;

	size_t capacity() const; //ready
	size_t cardianlity(Trit value) const;
	std::unordered_map< Trit, int, std::hash<int> > cardinality();
	void shrink(); //drop TritSet size_ to start value or to value enough to store set trit.
	void trim(size_t lastIndex);
	size_t length() const;


	~TritSet();
};

void resizeArray(uint* set, size_t capacity);
void copyArray(uint* from, uint* to, size_t size);

