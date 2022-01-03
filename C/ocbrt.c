/**
* Copyright Johannes Kloimböck 2020 - 2022.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "oroots.h"
#include <stdbool.h>

#define ONE_THIRD 0.333333333333333333
#define TWO_THIRDS 0.666666666666666667
#define FOUR_THIRDS 1.3333333333333333333

double orcbrt(const double a) {
	double_ull val;
	val.d = a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;

	val.ull ^= sign;

#if CHECK_SPECIAL_CASES != 0
	if (val.ull == 0ULL) {
		return 1.0 / a;
	}
#endif

	unsigned long long exponent = val.ull;
	int iterations = 3;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !(exponent & 0x7ff0000000000000ULL);
#endif

	exponent = -19178652474277888 - exponent;
	exponent /= 3ULL;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		iterations = 32;
	}
#endif

	double_ull guess;
	guess.ull = exponent;

	double thirdA = val.d * ONE_THIRD;

	for (int i = 0; i < iterations; ++i) {
		guess.d *= (FOUR_THIRDS - (thirdA * guess.d) * (guess.d * guess.d));
	}

	corr_t g = (corr_t)(guess.d);
	g *= ((corr_t)(FOUR_THIRDS) - ((corr_t)(val.d) * ONE_THIRD) * (g * g) * g);
	guess.d = (double)(g);

	guess.ull = val.ull == DOUBLE_INF ? 0ULL : guess.ull;
	guess.ull |= sign;

	return guess.d;
}

#if ONLY_USE_RECIP_ROOTS == 0
double ocbrt(const double a) {
	double_ull val;
	val.d = a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;

	val.ull ^= sign;

#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0 || a == -0.0 || val.ull == DOUBLE_INF) {
		return a;
	}
#endif

	unsigned long long exponent = val.ull;

	int iterations = 3;
#if SUBNORMAL_NUMBERS != 0
	if (!(exponent & 0x7ff0000000000000ULL)) {
		iterations = 32;
	}
#endif

	exponent /= 3ULL;
	exponent += 0x2a9f5cc62cb0f9e1ULL;

	double_ull guess;
	guess.ull = exponent;

	double one_third_a = val.d * ONE_THIRD;

	for (int i = 0; i < iterations; ++i) {
		guess.d = guess.d * TWO_THIRDS + (one_third_a / (guess.d * guess.d));
	}

	corr_t g = (corr_t)(guess.d);
	corr_t guess_sqr = g * g;
	guess.d = (double)(g * TWO_THIRDS + ((corr_t)(val.d) * ONE_THIRD / (guess_sqr)));
	guess.ull |= sign;

	return guess.d;
}
#else
inline double ocbrt(const double a) {
	return 1.0 / orcbrt(a);
}
#endif

float orcbrtf(const float a) {
	float_ul val;
	val.f = a;

	unsigned long sign = val.ul & 0x80000000UL;

	val.ul ^= sign;

#if CHECK_SPECIAL_CASES != 0
	if (val.ul == 0UL) {
		return 1.0f / a;
	}
#endif

	unsigned long exponent = val.ul;
	int iterations = 3;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !(exponent & 0x7f800000UL);
#endif

	exponent = 0xfdde0001UL - exponent;
	exponent /= 3UL;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		iterations = 32;
	}
#endif

	float_ul guess;
	guess.ul = exponent;

	float thirdA = val.f * (float)(ONE_THIRD);

	for (int i = 0; i < iterations; ++i) {
		guess.f *= ((float)(FOUR_THIRDS) - thirdA * guess.f * guess.f * guess.f);
	}

	corrf_t g = (corrf_t)(guess.f);
	g *= ((corrf_t)(FOUR_THIRDS) - ((corrf_t)(val.f) * ONE_THIRD) * (g * g) * g);
	guess.f = (float)(g);

	guess.ul = val.ul == FLOAT_INF ? 0UL : guess.ul;
	guess.ul |= sign;

	return guess.f;
}

#if ONLY_USE_RECIP_ROOTS == 0
float ocbrtf(const float a) {
	float_ul val;
	val.f = a;

	unsigned long sign = val.ul & 0x80000000UL;

	val.ul ^= sign;

#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0f || a == -0.0f || val.ul == FLOAT_INF) {
		return a;
	}
#endif

	unsigned long exponent = val.ul;
	int iterations = 2;

#if SUBNORMAL_NUMBERS != 0
	if (!(exponent & 0x7f800000UL)) {
		iterations = 31;
	}
#endif

	exponent /= 3UL;
	exponent += 0x2a501a5bUL;

	float_ul guess;
	guess.ul = exponent;

	float one_third_a = val.f * (float)(ONE_THIRD);

	for (int i = 0; i < iterations; ++i) {
		guess.f = guess.f * (float)(TWO_THIRDS) + (one_third_a / (guess.f * guess.f));
	}

	corrf_t g = (corrf_t)(guess.f);
	corrf_t guess_sqr = g * g;
	guess.f = (float)(g * TWO_THIRDS + ((corrf_t)(val.f) * ONE_THIRD / (guess_sqr)));
	guess.ul |= sign;

	return guess.f;
}
#else
inline float ocbrtf(const float a) {
	return 1.0f / orcbrtf(a);
}
#endif
