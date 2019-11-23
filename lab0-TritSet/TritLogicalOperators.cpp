#include "pch.h"
#include "TritSet.h"
#include <algorithm>

//TritSet TritSet::operator&(TritSet const& A) const
//{
//	TritSet C = capacity() > A.capacity() ? *this : A;
//	for (size_t i = 0; i < std::min(capacity(), A.capacity()); i++) {
//		C.tritSet_[i] = tritSet_[i] & A.tritSet_[i];
//	}
//	return C;
//}
//
TritSet operator|(TritSet const& A, TritSet const& B) 
{
	TritSet C = A.capacity() > B.capacity() ? A : B;
	for (size_t i = 0; i < std::min(A.capacity(), B.capacity()); i++) {
		C[i] = A[i] | B[i];
	}
	return C;
}
//
//TritSet TritSet::operator~() const
//{
//	TritSet C = *this;
//	for (size_t i = 0; i <= ind_last_notional_; i++) {
//		if (C.getValue(i) == True) {
//			C.setValue(False, i);
//		}
//		else if (C.getValue(i) == False) {
//			C.setValue(True, i);
//		}
//	}
//	return C;
//}
