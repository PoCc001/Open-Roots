/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "osqrt.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

double csqrt(const double a) {
	if (a < 0.0) {
		double nan;
		get_nan(&nan);
		return nan;
	}

	if (a == 0.0 || a == 1.0 || a != a) {
		return a;
	}

	double_ull val;
	val.d = a;

	unsigned int exponent = (unsigned int)(val.ull >> 52);

	bool is_sub_normal = !exponent;

	if (exponent & DOUBLE_EXP_MASK_1) {
		exponent &= DOUBLE_EXP_MASK_2;
		exponent >>= 1;
		exponent |= DOUBLE_EXP_MASK_1;
	}
	else {
		unsigned int exponent2 = DOUBLE_EXP_MASK_1 - exponent;
		exponent2 >>= 1;
		exponent = DOUBLE_EXP_MASK_1 - exponent2;
	}

	if (is_sub_normal) {
		unsigned long long mantissa = val.ull & DOUBLE_MANTISSA_MASK;
		unsigned int sub_normal_exponent = leading_zeros_ull(&mantissa) - 11;
		sub_normal_exponent >>= 1;
		exponent -= sub_normal_exponent;
	}

	double_ull guess;
	guess.ull = (unsigned long long)(exponent) << 52;

	for (int i = 0; i < 5; ++i) {
	//	guess.d = (guess.d + (a / guess.d)) / 2.0;
		guess.d += (a / guess.d);
		guess.ull -= 0x10000000000000ULL;
	}

	if ((guess.d * guess.d) > a) {
		--guess.ull;
	}

	if ((guess.d * guess.d) < a) {
		++guess.ull;
	}

	return guess.d;
}