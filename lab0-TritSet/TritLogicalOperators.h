#pragma once

#include "TritSet.h"

TritSet operator&(TritSet const& A, TritSet const& B);

TritSet operator|(TritSet const& A, TritSet const& B);

TritSet operator~(TritSet const& A);