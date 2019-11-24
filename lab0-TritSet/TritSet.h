#pragma once
#include <unordered_map>
#include "Trit.h"
#include "ProxyTrit.h"

typedef unsigned uint;

class TritSet
{
public:
	explicit TritSet(size_t capacity);
	TritSet(TritSet const& set);
	TritSet(ProxyTrit const& set);

	TritSet& operator=(TritSet const& set);
	ProxyTrit& operator[](size_t index);

	TritSet operator&(TritSet const& A) const;
	TritSet operator|(TritSet const& A) const;
	TritSet operator~() const;

	size_t capacity() const { return set_.capacity(); }
	size_t cardianlity(Trit value) const { return set_.cardianlity(value); }
	std::unordered_map< Trit, int, std::hash<int> > cardinality() const { return set_.cardinality(); }
	void shrink() { set_.shrink(); } //drop TritSet size_ to start value or to value enough to store set trit.
	void trim(size_t lastIndex) { set_.trim(lastIndex); }
	size_t length() const { return set_.length(); }

	void setRand() { set_.setRand(); }
private:
	ProxyTrit set_;
};


