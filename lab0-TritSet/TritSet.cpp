#include "pch.h"
#include "TritSet.h"
#include "CalcArrSize.h"
#include <algorithm>

#define byte 8
#define trit_bit_size 2

TritSet::TritSet(TritSet const& set)
	: startArraySize_(set.startArraySize_)
	, arraySize_(set.arraySize_)
	, ind_last_notional_(set.ind_last_notional_)
	, countFalse_(set.countFalse_)
	, countTrue_(set.countTrue_)
	, tritSet_(new uint[arraySize_])
{
	copyArray(set.tritSet_, tritSet_, arraySize_);
}

ProxyTrit TritSet::operator[](size_t index) const
{
	return ProxyTrit(*this, index);
}

ProxyTrit TritSet::operator[](size_t index)
{
	return ProxyTrit(*this, index);
}

size_t TritSet::length() const {
	return ind_last_notional_ + 1;
}

void TritSet::shrink() {
	resizeTrit(*this, std::max(this->ind_last_notional_, this->startArraySize_));
}

size_t TritSet::cardianlity(Trit trit) const
{
	switch (trit) {
	case False: return countFalse_;
	case Unknown: return ind_last_notional_ - countFalse_ - countTrue_;
	case True: return countTrue_;
	}
}

void TritSet::trim(size_t lastIndex)
{
	resizeTrit(*this, lastIndex);
}

TritSet& TritSet::operator=(TritSet const& set)
{
	if (this != &set) {
		delete[] tritSet_;
		arraySize_ = set.arraySize_;
		tritSet_ = new uint[arraySize_];
		copyArray(set.tritSet_, tritSet_, arraySize_);
	}
	return *this;
}

// Return trit position in array.
void calcTritPalce(size_t& arrayElem, size_t& posInUint, uint& mask, size_t const index)
{
	arrayElem = calcArrSize(index) - 1;
	posInUint = trit_bit_size * index % (byte * sizeof(uint));
	mask = (3 << (posInUint / trit_bit_size));
}

// Return value index trit
Trit TritSet::getValue(size_t const index)
{
	size_t arrayElem;
	size_t posInUint;
	uint mask;
	calcTritPalce(arrayElem, posInUint, mask, index);
	return (Trit)((tritSet_[arrayElem] & mask) >> (posInUint / trit_bit_size));
}

// Set trit index number on value;
void TritSet::setValue(Trit const value, size_t const index)
{
	size_t arrayElem;
	size_t posInUint;
	uint mask;
	calcTritPalce(arrayElem, posInUint, mask, index);
	if (value == False) {
		++countFalse_;
	}
	else if (value == True) {
		++countTrue_;
	}
	if (getValue(index) == True) {
		--countTrue_;
	}
	else if (getValue(index) == False) {
		--countFalse_;
	}
	tritSet_[arrayElem] = (tritSet_[arrayElem] & ~mask) | (value << (posInUint / trit_bit_size));
	if (ind_last_notional_ < index && value != Unknown) {
		ind_last_notional_ = index;
	}
	else if (ind_last_notional_ == index && value == Unknown) {
		for (size_t i = index; i >= 0; --i) {
			if (getValue(i) != Unknown ) {
				ind_last_notional_ = i;
				break;
			}
		}
	}
}

// Change size of array. If capacity is shorter then it was resizeTrit delite oversize piece
void resizeTrit(TritSet& set, size_t capacity)
{
	size_t size = calcArrSize(capacity);
	uint* newArray = new uint[size];
	copyArray(set.tritSet_, newArray, size);

	set.arraySize_ = size;
	set.tritSet_ = newArray;
}


// Make array of Unknow
TritSet::TritSet(size_t capacity)
{
	size_t size = calcArrSize(capacity);
	tritSet_ = new uint[size];
	char byteOfUnknow = 0b01010101;
	uint tmp = byteOfUnknow;
	for (size_t i = 0; i < sizeof(uint) - 1; i++) {
		tmp = tmp << byte;
		tmp |= byteOfUnknow;
	}
	for (size_t i = 0; i < size; i++) {
		tritSet_[i] = tmp;
	}
	arraySize_ = size;
	ind_last_notional_ = 0;
	startArraySize_ = size;
	countFalse_ = 0;
	countTrue_ = 0;
}

//return arraySize_
size_t TritSet::capacity() const
{
	return arraySize_;
}

TritSet::~TritSet()
{
	delete[] tritSet_;
}

void copyArray(uint* from, uint* to, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		to[i] = from[i];
	}
}
