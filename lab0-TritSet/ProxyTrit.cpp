#include "pch.h"
#include "TritSet.h"
#include "ProxyTrit.h"
#include <algorithm>

#define byte 8
#define trit_bit_size 2

uint makeTmp()
{
	unsigned char byteOfUnknow = 0b01010101;
	uint tmp = byteOfUnknow;
	for (size_t i = 0; i < sizeof(uint) - 1; i++) {
		tmp = tmp << byte;
		tmp |= byteOfUnknow;
	}
	return tmp;
}

ProxyTrit::ProxyTrit(size_t capacity)
	: countFalse_(0)
	, countTrue_(0)
	, capacity_(capacity)
	, startArraySize_(capacity)
	, ind_last_notional_(0)
{
	size_t size = calcArrSize(capacity);
	tritSet_ = new uint[size];	
	std::fill(tritSet_, tritSet_ + size, makeTmp());
}



ProxyTrit::ProxyTrit(ProxyTrit const & set)
	:capacity_(set.capacity_)
	,tritSet_(new uint[calcArrSize(capacity_)])
	,startArraySize_(set.startArraySize_)
	,countTrue_(set.countTrue_)
	,countFalse_(set.countFalse_)
	,ind_last_notional_(set.ind_last_notional_)
{
	copyTrit(set);
}

void ProxyTrit::copyTrit(const ProxyTrit & set)
{
	for (size_t i = 0; i != calcArrSize(capacity_); i++) {
		tritSet_[i] = set.tritSet_[i];
	}
}

ProxyTrit & ProxyTrit::operator=(ProxyTrit const & set)
{
	if (this != &set) {
		countFalse_ = set.countFalse_;
		countTrue_ = set.countTrue_;
		ind_last_notional_ = set.ind_last_notional_;
		capacity_ = set.capacity_;
		delete[] tritSet_;
		tritSet_ = new uint[calcArrSize(capacity_)];
		copyTrit(set);
	}
	return *this;
}


void ProxyTrit::operator=(Trit value)
{
	if (index_ > capacity_) {
		if (value == Unknown) { return; }
		resizeTrit(index_);
		capacity_ = index_;
	}
	setValue(value, index_);
}


bool ProxyTrit::operator==(ProxyTrit set) const
{
	uint value1, value2;

	value1 = getValue(index_);
	value2 = set.getValue(index_);
	return value1 == value2;
}

bool ProxyTrit::operator==(Trit value) const
{
	return getValue(index_) == value;
}


bool ProxyTrit::operator!=(ProxyTrit value) const
{
	return !(this->operator==(value));
}


ProxyTrit ProxyTrit::operator|(ProxyTrit const& A) const
{
	ProxyTrit C(std::max(A.capacity_, capacity_));
	for (size_t i = 0; i < calcArrSize(capacity_); i++) {
		C.tritSet_[i] = tritSet_[i] | A.tritSet_[i];
	}
	return C;
}


ProxyTrit ProxyTrit::operator&(ProxyTrit const& A) const
{
	ProxyTrit C(std::max(A.capacity_, capacity_));
	for (size_t i = 0; i < calcArrSize(capacity_); i++) {
		C.tritSet_[i] = tritSet_[i] & A.tritSet_[i];
	}
	return C;
}


ProxyTrit ProxyTrit::operator~() const
{
	ProxyTrit C(capacity_);
	for (size_t i = 0; i < capacity_; i++) {
		Trit value = getValue(i);
		switch (value) {
		case False: C.setValue(True, i);
			break;
		case Unknown: C.setValue(Unknown, i);
			break;
		case True: C.setValue(False, i);
			break;
		default:
			break;
		}
	}
	return C;
}


void ProxyTrit::calcLastNotional(const size_t & tritsIndex)
{
	size_t cur_ind = tritsIndex;
	size_t tmp = makeTmp();
	for (size_t i = calcArrSize(tritsIndex) - 1; i >= 0; --i) {
		uint val = tritSet_[i];
		if (val != tmp || i == 0) {
			while (getValue(cur_ind) == Unknown && cur_ind != 0) {
				cur_ind--;
			}
			ind_last_notional_ = cur_ind;
			return;
		}
		cur_ind -= sizeof(uint)*byte/trit_bit_size;
	}
}


void ProxyTrit::setValue(Trit const value, size_t & const index)
{
	size_t arrayElem;
	size_t posInUint;
	uint mask;
	calcTritPalce(arrayElem, posInUint, mask, index);
	switch (value) {
	case True:
		++countTrue_;
		break;
	case False:
		++countFalse_;
		break;
	default:
		break;
	}
	if (getValue(index) == True) {
		--countTrue_;
	}
	else if (getValue(index) == False) {
		--countFalse_;
	}
	uint a = tritSet_[arrayElem] & ~mask;
	uint b = value << posInUint;
	tritSet_[arrayElem] = a | b;
	if (ind_last_notional_ < index && value != Unknown) {
		ind_last_notional_ = index;
	}
	else if (ind_last_notional_ == index && value == Unknown) {
		calcLastNotional(index);
	}
}


// Return value index trit
Trit ProxyTrit::getValue(size_t const index) const
{
	if (index > capacity_) {
		return Unknown;
	}
	size_t arrayElem;
	size_t posInUint;
	uint mask;
	calcTritPalce(arrayElem, posInUint, mask, index);
	uint a = (tritSet_[arrayElem] & mask);
	uint out = (a >> posInUint);
	return (Trit)out;
}

// Return trit position in array.
void calcTritPalce(size_t& arrayElem, size_t& posInUint, uint& mask, size_t const index)
{
	arrayElem = calcArrSize(index) - 1;
	posInUint = trit_bit_size * (index) % (byte * sizeof(uint));
	mask = (0b11 << (posInUint));
}


void ProxyTrit::updCountTF(size_t &curVal)
{
	for (size_t j = 0; j < sizeof(uint) / trit_bit_size; j++) {
		Trit trit = (Trit)(curVal & 0b11);
		switch (trit) {
		case True: ++countTrue_; break;
		case False: ++countFalse_; break;
		default:
			break;
		}
		curVal >>= trit_bit_size;
	}
}


// Change size of array. If capacity is shorter then it was resizeTrit delite oversize piece
void ProxyTrit::resizeTrit(size_t capacity)
{
	size_t size = calcArrSize(capacity);
	uint* newArray = new uint[size];
	size_t len = std::min(calcArrSize(capacity_), size);
	size_t i;
	for ( i = 0; i < len; i++) {
		newArray[i] = tritSet_[i];
		size_t curVal = tritSet_[i];
		if (capacity < capacity_) {
			// Update countTrue and countFalse
			updCountTF(curVal);
		}
	}
	if (capacity < capacity_) {
		calcLastNotional(capacity);
	}
	else if (capacity > capacity_) {
		std::fill(newArray + len, newArray + size, makeTmp());
	}

	capacity_ = capacity;
	delete[] tritSet_;
	tritSet_ = newArray;
}


size_t ProxyTrit::cardianlity(Trit trit) const
{
	switch (trit) {
	case False: return countFalse_;
	case Unknown: return ind_last_notional_ - countFalse_ - countTrue_ + 1;
	case True: return countTrue_;
	}
}


std::unordered_map<Trit, int, std::hash<int>> ProxyTrit::cardinality() const
{
	std::unordered_map<Trit, int, std::hash<int>> tritsAmount;
	tritsAmount[True] = countTrue_;
	tritsAmount[False] = countFalse_;
	tritsAmount[Unknown] = ind_last_notional_ - countFalse_ - countTrue_ + 1;
	return tritsAmount;
}


void ProxyTrit::setRand()
{
	std::srand(time_t(NULL));
	for (size_t i = 0; i != calcArrSize(capacity_); i++) {
		tritSet_[i] = rand() / RAND_MAX * UINT_MAX;
		updCountTF(tritSet_[i]);
	}
	calcLastNotional(capacity_);
}
