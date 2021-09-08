/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "oroots.h"
#include <stdbool.h>

double orcbrt(const double a) {
	double_ull val;
	val.d = a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;

#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0) {
		double_ull inf;
		inf.ull = DOUBLE_INF ^ sign;
		return inf.d;
	}
#endif

	val.ull ^= sign;

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

	double thirdA = a * ONE_THIRD;

	for (int i = 0; i < iterations; ++i) {
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
	if (a == 0.0 || a == -0.0) {
		return a;
	}
#endif

	double_ull val;
	val.d = a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;

	val.ull ^= sign;

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
	float_ul val;
	val.f = a;

	unsigned long sign = val.ul & 0x80000000UL;

#if CHECK_SPECIAL_CASES != 0
	if (a == 0.0) {
		float_ul inf;
		inf.ul = FLOAT_INF ^ sign;
		return inf.f;
	}
#endif

	val.ul ^= sign;

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

	float thirdA = a * (float)(ONE_THIRD);

	for (int i = 0; i < iterations; ++i) {
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
	if (a == 0.0f || a == -0.0f) {
		return a;
	}
#endif

	float_ul val;
	val.f = a;

	unsigned long sign = val.ul & 0x80000000UL;

	val.ul ^= sign;

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
