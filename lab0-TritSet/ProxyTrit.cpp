#include "pch.h"
#include "TritSet.h"
#include "CalcArrSize.h"

#define byte 8
#define trit_bit_size 2

ProxyTrit::ProxyTrit(uint& tritSet, size_t index)
	: tritSet_(tritSet_)
	, index_(index)
{}

void ProxyTrit::operator=(Trit value)
{
	if (index_ > lastAvaliableIndex_) {
		if (value == Unknown) { return; }
		resizeArray(tritSet_, index_);
	}
	setValue(value, index_);
}

bool ProxyTrit::operator==(Trit value)
{
	if (calcArrSize(index_) > lastAvaliableIndex_) {
		return Unknown == value;
	}
	return getValue(index_) == value;
}

bool ProxyTrit::operator!=(Trit value) 
{
	return !(this->operator==(value));
}

uint ProxyTrit::operator|(ProxyTrit const& A) const
{
	size_t index = calcArrSize(index_) - 1;
	return ltritSet_.tritSet_[index] | A.ltritSet_.tritSet_[index];
}

uint ProxyTrit::operator&(ProxyTrit const& A) const
{
	size_t index = calcArrSize(index_) - 1;
	return ltritSet_.tritSet_[index] & A.ltritSet_.tritSet_[index];
}

uint ProxyTrit::operator~() const
{
	size_t index = calcArrSize(index_) - 1;
	return ~ltritSet_.tritSet_[index];
}

void ProxyTrit::setValue(Trit const value, size_t const index)
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
			if (getValue(i) != Unknown) {
				ind_last_notional_ = i;
				break;
			}
		}
	}
}

// Return value index trit
Trit ProxyTrit::getValue(size_t const index)
{
	size_t arrayElem;
	size_t posInUint;
	uint mask;
	calcTritPalce(arrayElem, posInUint, mask, index);
	return (Trit)((tritSet_[arrayElem] & mask) >> (posInUint / trit_bit_size));
}

// Return trit position in array.
void calcTritPalce(size_t& arrayElem, size_t& posInUint, uint& mask, size_t const index)
{
	arrayElem = calcArrSize(index) - 1;
	posInUint = trit_bit_size * index % (byte * sizeof(uint));
	mask = (3 << (posInUint / trit_bit_size));
}