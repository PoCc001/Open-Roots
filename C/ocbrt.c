/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "ocbrt.h"
#include <stdbool.h>

double ocbrt(const double a) {
#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0) {
		return a;
	}
#endif

	double_ull val;
	val.d = a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;

	val.ull &= 0x7fffffffffffffffULL;

	int exponent = (int)(val.ull >> 52);

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !exponent;
#endif

	exponent /= 3;
	exponent += 683;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		int sub_normal_exponent = leading_zeros_ull(&val.ull);
		sub_normal_exponent /= 3;
		exponent -= sub_normal_exponent;
	}
#endif

	val.ull |= sign;

	double_ull guess;
	guess.ull = (unsigned long long)(exponent) << 52;

	guess.ull |= sign;

	for (int i = 0; i < 5; ++i) {
		guess.d = (2.0 * guess.d + (val.d / (guess.d * guess.d))) * ONE_THIRD;
	}

	corr_t diff = ((corr_t)(guess.d) * (corr_t)guess.d * (corr_t)(guess.d)) - (corr_t)val.d;
	diff /= 3.0 * (corr_t)(guess.d) * (corr_t)(guess.d);
	guess.d -= (double)(diff);

	return guess.d;
}

extern float ocbrtf(const float a) {
#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0f) {
		return a;
	}
#endif

	float_ul val;
	val.f = a;

	unsigned long sign = val.ul & 0x80000000UL;

	val.ul &= 0x7ffffffffUL;

	int exponent = (int)(val.ul >> 23);

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !exponent;
#endif

	exponent /= 3;
	exponent += 85;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		int sub_normal_exponent = leading_zeros_ul(&val.ul);
		sub_normal_exponent /= 3;
		exponent -= sub_normal_exponent;
	}
#endif

	val.ul |= sign;

	float_ul guess;
	guess.ul = (unsigned long)(exponent) << 23;

	guess.ul |= sign;

	for (int i = 0; i < 4; ++i) {
		guess.f = (2.0f * guess.f + (val.f / (guess.f * guess.f))) * (float)(ONE_THIRD);
	}

	corrf_t diff = ((corrf_t)(guess.f) * (corrf_t)guess.f * (corrf_t)(guess.f)) - (corrf_t)val.f;
	diff /= 3.0f * (corrf_t)(guess.f) * (corrf_t)(guess.f);
	guess.f -= (float)(diff);

	return guess.f;
}
