/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "osqrt.h"
#include <stdbool.h>

double osqrt(const double a) {
	double_ull val;
	val.d = a;

	unsigned long long manipulated_exp = val.ull;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !((manipulated_exp) & (0x7ff0000000000000ULL));
#endif

	manipulated_exp >>= 1;
	manipulated_exp += 0x1ff0000000000000ULL;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		int sub_normal_exponent = leading_zeros_ull(&val.ull);
		sub_normal_exponent >>= 1;
		manipulated_exp = (unsigned long long)(-(long long)(sub_normal_exponent)) << 52;
		manipulated_exp &= 0x7fffffffffffffffULL;
	}
#endif

	double_ull guess;
	guess.ull = manipulated_exp;

	for (int i = 0; i < 3; ++i) {
		guess.d += (a / guess.d);
		guess.ull -= 0x10000000000000ULL;
	}

	double guesst2 = guess.d + (a / guess.d);
	guess.d = guesst2 * 0.5;

	corr_t diff = ((corr_t)(guess.d) * (corr_t)guess.d) - (corr_t)a;
	diff /= (corr_t)(guesst2);
	guess.d -= (double)(diff);

#if CHECK_SPECIAL_CASES != 0
	guess.ull = a > 0.0 ? guess.ull : DOUBLE_NAN;
	
	return a == 0.0 ? 0.0 : guess.d;
#else
	return guess.d;
#endif
}

float osqrtf(const float a) {
	float_ul val;
	val.f = a;

	unsigned long manipulated_exp = val.ul;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !((manipulated_exp) & (0x7f800000UL));
#endif

	manipulated_exp >>= 1;
	manipulated_exp += 0x1f800000UL;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		int sub_normal_exponent = leading_zeros_ul(&val.ul);
		sub_normal_exponent >>= 1;
		manipulated_exp = (unsigned long)(-(long)(sub_normal_exponent)) << 23;
		manipulated_exp &= 0x7fffffffUL;
	}
#endif

	float_ul guess;
	guess.ul = manipulated_exp;

	for (int i = 0; i < 2; ++i) {
		guess.f += (a / guess.f);
		guess.ul -= 0x300000UL;
	}

	float guesst2 = guess.f + (a / guess.f);
	guess.f = guesst2 * 0.5f;

	corrf_t diff = ((corrf_t)(guess.f) * (corrf_t)guess.f) - (corrf_t)a;
	diff /= (corrf_t)(guesst2);
	guess.f -= (float)(diff);

#if CHECK_SPECIAL_CASES != 0
	guess.ul = a > 0.0f ? guess.ul : FLOAT_NAN;

	return a == 0.0f ? 0.0f : guess.f;
#else
	return guess.f;
#endif
}
