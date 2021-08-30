/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "oroots.h"
#include <stdbool.h>

double orcbrt(const double a) {
#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0) {
		return DOUBLE_INF;
	}
#endif

	double_ull val;
	val.d = a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;

	val.ull ^= sign;

	int exponent = (int)(val.ull >> 52);

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !exponent;
#endif

	exponent = 4092 - exponent;
	exponent /= 3;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		if (val.ull < 3) {
			double_ull r;
			r.ull = val.ull == 1ULL ? 0x2990000000000000ULL : 0x299428a2f98d728bULL;
			r.ull |= sign;
			return r.d;
		}

		int sub_normal_exponent = leading_zeros_ull(&val.ull);
		sub_normal_exponent /= 3;
		exponent += sub_normal_exponent;
	}
#endif

	double_ull guess;
	guess.ull = (unsigned long long)(exponent) << 52;

	double thirdA = a * ONE_THIRD;

	for (int i = 0; i < 6; ++i) {
		guess.d *= (FOUR_THIRDS - (thirdA * guess.d) * (guess.d * guess.d));
	}

	corr_t g = (corr_t)(guess.d);
	g *= ((corr_t)(FOUR_THIRDS) - ((corr_t)(a) * ONE_THIRD) * (g * g) * g);
	guess.d = (double)(g);

	guess.ull |= sign;

	return guess.d;
}

double ocbrt(const double a) {
#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0) {
		return a;
	}
#endif

	double_ull val;
	val.d = a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;

	val.ull ^= sign;

	int exponent = (int)(val.ull >> 52);

	int iterations = 5;
#if SUBNORMAL_NUMBERS != 0
	if (!exponent) {
		iterations = 30;
	}
#endif

	exponent /= 3;
	exponent += 683;

	double_ull guess;
	guess.ull = (unsigned long long)(exponent) << 52;

	for (int i = 0; i < iterations; ++i) {
		guess.d = (2.0 * guess.d + (val.d / (guess.d * guess.d))) * ONE_THIRD;
	}

	corr_t guess_sqr = (corr_t)(guess.d) * (corr_t)(guess.d);
	corr_t diff = (guess_sqr * (corr_t)(guess.d)) - (corr_t)(val.d);
	diff /= (corr_t)(3.0) * guess_sqr;
	guess.d -= (double)(diff);
	guess.ull |= sign;

	return guess.d;
}

float orcbrtf(const float a) {
#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0) {
		return FLOAT_INF;
	}
#endif

	float_ul val;
	val.f = a;

	unsigned long sign = val.ul & 0x80000000UL;

	val.ul ^= sign;

	int exponent = (int)(val.ul >> 23);

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !exponent;
#endif

	exponent = 508 - exponent;
	exponent /= 3;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		if (val.ul < 3) {
			float_ul r;
			r.ul = val.ul == 1UL ? 0x26a14518UL : 0x26cb2ff5UL;
			r.ul |= sign;
			return r.f;
		}

		int sub_normal_exponent = leading_zeros_ul(&val.ul);
		sub_normal_exponent /= 3;
		exponent += sub_normal_exponent;
	}
#endif

	float_ul guess;
	guess.ul = (unsigned long)(exponent) << 23;

	float thirdA = a * (float)(ONE_THIRD);

	for (int i = 0; i < 5; ++i) {
		guess.f *= ((float)(FOUR_THIRDS) - thirdA * guess.f * guess.f * guess.f);
	}

	corrf_t g = (corrf_t)(guess.f);
	g *= ((corrf_t)(FOUR_THIRDS)-((corrf_t)(a) * ONE_THIRD) * (g * g) * g);
	guess.f = (float)(g);

	guess.ul |= sign;

	return guess.f;
}

float ocbrtf(const float a) {
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
	int iterations = 4;

#if SUBNORMAL_NUMBERS != 0
	if (!exponent) {
		iterations = 28;
	}
#endif

	exponent /= 3;
	exponent += 85;

	float_ul guess;
	guess.ul = (unsigned long)(exponent) << 23;

	for (int i = 0; i < iterations; ++i) {
		guess.f = (2.0f * guess.f + (val.f / (guess.f * guess.f))) * (float)(ONE_THIRD);
	}

	corrf_t guess_sqr = (corrf_t)(guess.f) * (corrf_t)(guess.f);
	corrf_t diff = (guess_sqr * (corrf_t)(guess.f)) - (corrf_t)(val.f);
	diff /= (corrf_t)(3.0) * guess_sqr;
	guess.f -= (float)(diff);
	guess.ul |= sign;

	return guess.f;
}
