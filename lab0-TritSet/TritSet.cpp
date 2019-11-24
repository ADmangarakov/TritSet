#include "pch.h"
#include "TritSet.h"
#include "ProxyTrit.h"
#include "CalcArrSize.h"

#define byte 8
#define trit_bit_size 2

TritSet::TritSet(TritSet const& set)
	:set_(set.set_)
{
}

TritSet::TritSet(ProxyTrit const& set)
	: set_(set)
{
}


TritSet::TritSet(size_t capacity)
	: set_(capacity)
{
}

TritSet& TritSet::operator=(TritSet const& set)
{
	if (this != &set) {
		set_.~ProxyTrit();
		set_ = set.set_;
	}
	return *this;
}

ProxyTrit& TritSet::operator[](size_t index)
{
	set_.index_ = index;
	return set_;
}


TritSet TritSet::operator&(TritSet const& A) const
{
	TritSet C = set_ & A.set_;
	return C;
}


TritSet TritSet::operator|(TritSet const& A) const
{
	TritSet C = set_ | A.set_;
	return C;
}


TritSet TritSet::operator~() const
{
	TritSet C = ~set_;
	return C;
}