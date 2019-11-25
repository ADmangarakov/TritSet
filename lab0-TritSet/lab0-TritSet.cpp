// lab0-TritSet.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <assert.h>
#include "TritSet.h"
#include "ProxyTrit.h"

int main()
{
	TritSet setA(100);
	setA.setRand();
	TritSet setB = setA;
	assert(setA.capacity() == setB.capacity());
	for (int i = 0; i < setB.capacity(); i++) {
		bool t = setA[i] == setB[i];
	}
	return 0;
}
