#pragma once

typedef unsigned uint;

enum Trit;

class ProxyTrit
{
	ProxyTrit(size_t capacity);
	~ProxyTrit();

	void operator=(Trit value) {

	}

	uint* tritSet_;
};

