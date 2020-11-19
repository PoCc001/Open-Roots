/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "ocbrt.h"
#include <stdbool.h>

double ccbrt(const double a) {
	double absA = a > 0.0 ? a : -a;
	if (absA == 0.0 || absA == 1.0 || a != a) {
		return a;
	}

	double_ull val;
	val.d = absA;

	unsigned int exponent = (unsigned int)(val.ull >> 52);

	bool is_sub_normal = !exponent;

	if (exponent & DOUBLE_EXP_MASK_1) {
		exponent &= DOUBLE_EXP_MASK_2;
		exponent /= 3;
		exponent |= DOUBLE_EXP_MASK_1;
	}
	else {
		unsigned int exponent2 = DOUBLE_EXP_MASK_1 - exponent;
		exponent2 /= 3;
		exponent = DOUBLE_EXP_MASK_1 - exponent2;
	}

	if (is_sub_normal) {
		unsigned long long mantissa = val.ull & DOUBLE_MANTISSA_MASK;
		unsigned int sub_normal_exponent = leading_zeros_ull(&mantissa) - 11;
		sub_normal_exponent /= 3;
		exponent -= sub_normal_exponent;
	}

	double_ull guess;
	guess.ull = (unsigned long long)(exponent) << 52;

	for (int i = 0; i < 6; ++i) {
		guess.d = (2.0 * guess.d + (absA / (guess.d * guess.d))) / 3.0;
	}

	if ((guess.d * guess.d * guess.d) > absA) {
		--guess.ull;
	}

	if ((guess.d * guess.d * guess.d) < absA) {
		++guess.ull;
	}

	if (a < 0.0) {
		guess.d = -guess.d;
	}

	return guess.d;
}