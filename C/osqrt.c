/**
* Copyright Johannes Kloimböck 2020 - 2022.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "oroots.h"
#include <stdbool.h>

inline void orsqrt_special_cases(const double a, double_ull* val, double_ull* guess) {
	guess->ull = a == 0.0 ? DOUBLE_INF : guess->ull;
	guess->ull = val->ull == DOUBLE_INF ? 0ULL : guess->ull;
}

void orsqrt_calc(const double a, double_ull* val, double_ull* guess) {
	val->d = a;

	uint64_t manipulated_exp = val->ull;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !((manipulated_exp) & (0x7ff0000000000000ULL));
#endif

	manipulated_exp = 0xbfcc409b00000000ULL - manipulated_exp;
	manipulated_exp >>= 1;
	int iterations = 3;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		iterations = 30;
	}
#endif

	guess->ull = manipulated_exp;
	double half_a = a * 0.5;
	
	for (int i = 0; i < iterations; ++i) {
		guess->d *= (1.5 - (half_a * guess->d * guess->d));
	}

	corr_t g = (corr_t)(guess->d);
	corr_t r = g * ((corr_t)(1.5) - ((corr_t)(a) * (corr_t)(0.5) * g * g));

	guess->d = (double)(r);

#if CHECK_SPECIAL_CASES != 0
	guess->ull = a >= 0.0 ? guess->ull : DOUBLE_NAN;
#endif
}

double orsqrt(const double a) {
	double_ull val;
	double_ull guess;
	orsqrt_calc(a, &val, &guess);
	orsqrt_special_cases(a, &val, &guess);

	return guess.d;
}

#if ONLY_USE_RECIP_ROOTS == 0
double osqrt(const double a) {
	double_ull val;
	val.d = a;

	uint64_t manipulated_exp = val.ull;
	int iterations = 2;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !((manipulated_exp) & (0x7ff0000000000000ULL));

	if (is_sub_normal) {
		iterations = 30;
	}
#endif

	manipulated_exp >>= 1;
	manipulated_exp += 0x1ff62ddf00000000ULL;

	double_ull guess;
	guess.ull = manipulated_exp;

	for (int i = 0; i < iterations; ++i) {
		guess.d += (a / guess.d);
		guess.d *= 0.5;
	}

	double guesst2 = guess.d + (a / guess.d);
	guess.d = guesst2 * 0.5;

	corr_t diff = ((corr_t)(guess.d) * (corr_t)guess.d) - (corr_t)a;
	diff /= (corr_t)(guesst2);
	guess.d -= (double)(diff);

#if CHECK_SPECIAL_CASES != 0
	guess.ull = a > 0.0 ? guess.ull : DOUBLE_NAN;
	guess.ull = val.ull == DOUBLE_INF ? DOUBLE_INF : guess.ull;
	return a == 0.0 ? 0.0 : guess.d;
#else
	return guess.d;
#endif
}
#else
inline double osqrt(const double a) {
	double_ull val;
	double_ull guess;
	orsqrt_calc(a, &val, &guess);

	return a * guess.d;
}
#endif


#if !defined(USE_TABLES_FOR_FLOAT) || USE_TABLES_FOR_FLOAT == 0

inline void orsqrtf_special_cases(const float a, float_ul* val, float_ul* guess) {
	guess->ul = a == 0.0f ? FLOAT_INF : guess->ul;
	guess->ul = val->ul == FLOAT_INF ? 0ULL : guess->ul;
}

inline void orsqrtf_calc(const float a, float_ul* val, float_ul* guess) {
	val->f = a;

	uint32_t manipulated_exp = val->ul;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !((manipulated_exp) & (0x7f800000UL));
#endif

	manipulated_exp = 0xbe700000UL - manipulated_exp;
	manipulated_exp >>= 1;
	int iterations = 2;

#if SUBNORMAL_NUMBERS != 0
	if (is_sub_normal) {
		iterations = 30;
	}
#endif

	guess->ul = manipulated_exp;
	float half_a = a * 0.5f;

	for (int i = 0; i < iterations; ++i) {
		guess->f *= (1.5f - (half_a * guess->f * guess->f));
	}

	corrf_t g = (corrf_t)(guess->f);
	corrf_t r = g * ((corrf_t)(1.5) - ((corrf_t)(a) * (corrf_t)(0.5) * g * g));

	guess->f = (float)(r);

#if CHECK_SPECIAL_CASES != 0
	guess->ul = a >= 0.0f ? guess->ul : FLOAT_NAN;
#endif
}

float orsqrtf(const float a) {
	float_ul val;
	float_ul guess;
	orsqrtf_calc(a, &val, &guess);
	orsqrtf_special_cases(a, &val, &guess);

	return guess.f;
}

#if ONLY_USE_RECIP_ROOTS == 0
float osqrtf(const float a) {
	float_ul val;
	val.f = a;

	uint32_t manipulated_exp = val.ul;
	int iterations = 1;

#if SUBNORMAL_NUMBERS != 0
	bool is_sub_normal = !((manipulated_exp) & (0x7f800000UL));

	if (is_sub_normal) {
		iterations = 33;
	}
#endif

	manipulated_exp >>= 1;
	manipulated_exp += 0x1f94da6dUL;

	float_ul guess;
	guess.ul = manipulated_exp;

	for (int i = 0; i < iterations; ++i) {
		guess.f += (a / guess.f);
		guess.f *= 0.5f;
	}

	float guesst2 = guess.f + (a / guess.f);
	guess.f = guesst2 * 0.5f;

	corrf_t diff = ((corrf_t)(guess.f) * (corrf_t)guess.f) - (corrf_t)a;
	diff /= (corrf_t)(guesst2);
	guess.f -= (float)(diff);

#if CHECK_SPECIAL_CASES != 0
	guess.ul = a > 0.0f ? guess.ul : FLOAT_NAN;
	guess.ul = val.ul == FLOAT_INF ? FLOAT_INF : guess.ul;
	return a == 0.0f ? 0.0f : guess.f;
#else
	return guess.f;
#endif
}
#else
inline float osqrtf(const float a) {
	float_ul val;
	float_ul guess;
	orsqrtf_calc(a, &val, &guess);

	return a * guess.f;
}
#endif

#endif