#include "pch.h"
#include "../lab0-TritSet/TritSet.h"
#include "../lab0-TritSet/ProxyTrit.h"
#include "../lab0-TritSet/Trit.h"
#include <random>
TEST(MemoryTest, CapasityTest)
{
	//резерв памяти для хранения 1000 тритов
	TritSet set(1000);
	// length of internal array
	size_t allocLength = set.capacity();
	EXPECT_TRUE(allocLength >= 1000 * 2 / 8 / sizeof(uint));
	// 1000*2 - min bits count
	// 1000*2 / 8 - min bytes count
	// 1000*2 / 8 / sizeof(uint) - min uint[] size

	//не выделяет никакой памяти
	set[1000'000'000] = Unknown;
	EXPECT_TRUE(allocLength == set.capacity());

	// false, but no exception or memory allocation
	if (set[2000'000'000] == True) {}
	EXPECT_TRUE(allocLength == set.capacity());

	//выделение памяти
	set[1000'000'000] = True;
	EXPECT_TRUE(allocLength < set.capacity());

	//no memory operations
	allocLength = set.capacity();
	set[1000'000'000] = Unknown;
	set[1000'000] = False;
	EXPECT_TRUE(allocLength == set.capacity());

	//освобождение памяти до начального значения или 
	//до значения необходимого для хранения последнего установленного трита
	//в данном случае для трита 1000'000
	set.shrink();
	EXPECT_TRUE(allocLength > set.capacity());

	//освобождение памяти от 200го трита и дальше 
	set.trim(200);
	EXPECT_TRUE(set.capacity() <= 200 * 2 / 8 / sizeof(uint));
}
TEST(OperatorsTest, IndexSetGetTest)
{
	const size_t size = 100;
	TritSet a(size);
	for (int i = 0; i < size; i++) {
		uint A = ((i*i + 5 * i + 7 * i) % 4);
		A = A == 0b10 ? 0b01 : A;
		a[i] = (Trit)A;
	}
	for (int i = 0; i < size; i++) {
		uint A = ((i*i + 5 * i + 7 * i) % 4);
		A = A == 0b10 ? 0b01 : A;
		EXPECT_TRUE(a[i] == (Trit)A);
	}
}
TEST(OperatorsTest, EquatingTest)
{
	TritSet setA(100);
	setA.setRand();
	TritSet setB = setA;
	EXPECT_TRUE(setA.capacity() == setB.capacity());
	for (int i = 0; i < setB.capacity(); i++)
		EXPECT_TRUE(setA[i] == setB[i]);
}
TEST(OperatorsTest, LogicalAndTest)
{
	TritSet setA(100);
	setA.setRand();
	TritSet setB(200);
	setB.setRand();
	TritSet setC = setA & setB;
	EXPECT_TRUE(setC.capacity() == setB.capacity());
	for (int i = 0; i < setC.capacity(); i++)
		EXPECT_TRUE((setA[i] & setB[i]) == setC[i]);
}

TEST(OperatorsTest, LogicalOrTest)
{
	TritSet setA(100);
	setA.setRand();
	TritSet setB(200);
	setB.setRand();
	TritSet setC = setA | setB;
	EXPECT_TRUE(setC.capacity() == setB.capacity());
	for (int i = 0; i < setC.capacity(); i++)
		EXPECT_TRUE((setA[i] | setB[i]) == setC[i]);
}

TEST(OperatorsTest, LogicalNotTest)
{
	TritSet setA(100);
	setA.setRand();
	TritSet setB = ~setA;
	EXPECT_TRUE(setA.capacity() == setB.capacity());
	for (int i = 0; i < setB.capacity(); i++)
		EXPECT_TRUE(~setA[i] == setB[i]);
}
TEST(CardinalityTest, size_tTest)
{
	TritSet setA(100);
	for (size_t i = 0; i < 30; i++)
		setA[i] = True;
	for (size_t i = 60; i < 90; i++)
		setA[i] = False;
	EXPECT_EQ(setA.cardianlity(True), 30);
	EXPECT_EQ(setA.cardianlity(False), 30);
	EXPECT_EQ(setA.cardianlity(Unknown), 30);
}

TEST(CardinalityTest, mapTest)
{
	TritSet setA(100);
	for (size_t i = 0; i < 30; i++)
		setA[i] = True;
	for (size_t i = 60; i < 90; i++)
		setA[i] = False;
	auto m = setA.cardinality();
	EXPECT_EQ(m[True], 30);
	EXPECT_EQ(m[False], 30);
	EXPECT_EQ(m[Unknown], 30);
}