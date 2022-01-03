/**
* Copyright Johannes Kloimböck 2020 - 2022.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "oroots.h"
#include <stdbool.h>
#include <math.h>

#define DOUBLE_EXP_MASK_1 0b10000000000
#define DOUBLE_EXP_MASK_2 0b1111111111
#define DOUBLE_MANTISSA_MASK 0b1111111111111111111111111111111111111111111111111111ULL
#define DOUBLE_EXP_MASK_3 0b11111111111

#define FLOAT_EXP_MASK_1 0b10000000
#define FLOAT_EXP_MASK_2 0b1111111
#define FLOAT_MANTISSA_MASK 0b11111111111111111111111
#define FLOAT_EXP_MASK_3 0b11111111

inline double int_pow(const double *a, unsigned int n) {
	double result = (n & 1) ? *a : 1.0;
	n >>= 1;
	int length = (sizeof(int) * 8) - leading_zeros_ui(&n);
	double sqr = (*a) * (*a);
	for (int i = 0; i < length; ++i) {
		if (n & 1) {
			result *= sqr;
		}

		sqr *= sqr;
		n >>= 1;
	}

	return result;
}

inline void invroot(double *root, const double *a, const int *n) {
#if CHECK_SPECIAL_CASES != 0
	if (*a < 0.0 && (*n & 1) == 0) {
		double_ull nan;
		nan.ull = DOUBLE_NAN;
		*root = nan.d;
		return;
	}

	if (*a != *a || *a == 1.0 || *a == 0.0 || *a == -1.0) {
		*root = 1.0 / *a;
		return;
	}
#endif

	double_ull val;
	val.d = *a;

	unsigned long long sign = val.ull & 0x8000000000000000ULL;
	val.ull ^= sign;

	unsigned long long exp = val.ull;
	exp -= (1023ULL << 52);
	exp /= (long long)(*n);
	exp += (1023ULL << 52);

	double_ull guess;
	guess.ull = exp;

	guess.d = 1.0 / guess.d;
	
	for (int i = 0; i < 5; ++i) {
		guess.d = (((*n) + 1) * guess.d - (val.d * (int_pow(&guess.d, (*n) + 1)))) / *n;
	}

	guess.ull |= sign;
	*root = guess.d;

	return;
}

double oroot(double a, int n) {
	if (n == 1) {
		return a;
	}
	else if (n == -1) {
		return 1.0 / a;
	}
	else if (n == 2) {
		return osqrt(a);
	}
	else if (n == 3) {
		return ocbrt(a);
	}
	else if (n == 4) {
		return orsqrt(orsqrt(a));
	}
	else if (n > 0) {
		double r;
		invroot(&r, &a, &n);
		if (a < 0.0 && !(n & 1)) {
			double_ull nan;
			nan.ull = DOUBLE_NAN;
			return nan.d;
		}
		r = a == (1.0E300 * 1.0E300) ? a : (a == (-1.0E300 * 1.0E300) ? a : 1.0 / r);

		return r;
	}
	else if (n < 0) {
		double r;
		n = -n;
		invroot(&r, &a, &n);
		if (a < 0.0 && !(n & 1)) {
			double_ull nan;
			nan.ull = DOUBLE_NAN;
			return nan.d;
		}
		r = a == (1.0E300 * 1.0E300) ? 0.0 : (a == (-1.0E300 * 1.0E300) ? -0.0 : r);
	}
	else {
		return (a < 1.0 || a > -1.0 ? 0.0 : (1.0E300 * 1.0E300)) * a;
	}
}

float orootf(float a, int n) {
	return (float)(oroot((double)(a), n));
}
