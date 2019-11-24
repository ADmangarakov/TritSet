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
	for (size_t i = 0; i < 40; i++)
		setA[i] = True;
	for (size_t i = 60; i < 90; i++)
		setA[i] = False;
	assert(setA.cardianlity(False) == 30);
	assert(setA.cardianlity(Unknown) == 20);
	assert(setA.cardianlity(True) == 40);
	return 0;
}
