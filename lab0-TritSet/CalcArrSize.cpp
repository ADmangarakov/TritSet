#include "pch.h"
#include "CalcArrSize.h"

size_t calcArrSize(size_t capacity)
{
	capacity = !capacity ? 1 : capacity;
	size_t bit_size = trit_bit_size * capacity;
	size_t bit_in_uint = byte * sizeof(uint);
	/*size_t size = bit_size % bit_in_uint ? (bit_size / bit_in_uint) + 1
		: bit_size / bit_in_uint;*/
	size_t size = (bit_size / bit_in_uint) + 1;
	return size;
}