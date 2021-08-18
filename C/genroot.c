/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "oroots.h"
#include <stdbool.h>

inline double int_pow(const double *a, unsigned int n) {
	double result = (n & 1) ? *a : 1.0;
	n >>= 1;
	int length = (sizeof(int) * 8) - leading_zeros_ui(&n);
	double sqr = *a * *a;
	for (int i = 0; i < length; ++i) {
		if (n & 1) {
			result *= sqr;
		}

		sqr *= sqr;
		n >>= 1;
	}

	return result;
}

inline double int_pow_cbv(double a, unsigned int n) {
	double result = (n & 1) ? a : 1.0;
	n >>= 1;
	int length = (sizeof(int) * 8) - leading_zeros_ui(&n);
	double sqr = a * a;
	for (int i = 0; i < length; ++i) {
		if (n & 1) {
			result *= sqr;
		}

		sqr *= sqr;
		n >>= 1;
	}

	return result;
}

inline double exp_small(const double a) {
	double result = 1.0;
	double term = 1.0;
	double k = 1.0;
	while (term > 1E-10 || term < -1E-10) {
		term *= a;
		term /= k;
		result += term;
		k += 1.0;
	}

	return result;
}

inline double log_approx(double_ull a) {
	int exponent = (int)(a.ull >> 52) & DOUBLE_EXP_MASK_3;
	int powerOfTwo = exponent - DOUBLE_EXP_MASK_1;
	a.ull &= DOUBLE_MANTISSA_MASK;
	a.ull |= 0b100000000000000000000000000000000000000000000000000000000000000;
	double sum = (a.d - 1.0) / (a.d + 1.0);
	double term = sum;
	double j = 1.0;
	for (int i = 0; i < 25; i++) {
		term *= j * (a.d - 1.0) * (a.d - 1.0);
		j += 2.0;
		term /= (j * (a.d + 1.0) * (a.d + 1.0));
		sum += term;
	}

	return sum * 2.0 + powerOfTwo * LOG_2;
}

inline unsigned long long int_part(double_ull *real_number) {
	if (real_number->d < 1.0 && real_number->d > -1.0) {
		return 0;
	}
	else {
		unsigned int exponent = ((unsigned int)(real_number->ull >> 52) + 2) & DOUBLE_EXP_MASK_2;
		unsigned long long mantissa = real_number->ull & DOUBLE_MANTISSA_MASK;
		unsigned long long int_number = mantissa >> (52 - exponent);
		int_number |= (1ULL << exponent);
		return int_number;
	}
}

inline void pow_real(double *result, const double *a, const double *b) {
	unsigned long long int_p;
	double_ull b_d_ull;
	b_d_ull.d = *b;
	int_p = int_part(&b_d_ull);
	double rationalPart = *b - (double)(int_p);
	double_ull a_d_ull;
	a_d_ull.d = *a;
	double log_a = log_approx(a_d_ull);
	*result = exp_small(rationalPart * log_a);
	double ip = int_pow_cbv(E, (unsigned int)int_p);
	*result *= ip;
}

inline void invroot(double *root, const double *a, const int *n, bool inv) {
#if CHECK_SPECIAL_CASES != 0
	if (*a < 0.0 && (*n & 1) == 0) {
		double_ull nan;
		nan.ull = DOUBLE_NAN;
		*root = nan.d;
		return;
	}

	if (*a != *a || *a == 1.0 || *a == 0.0 || *a == -1.0) {
		*root = *a;
		return;
	}
#endif

	bool negative = *a < 0.0;

	double abs_a = negative ? -*a : *a;

	double_ull guess;
	guess.d = 1.0;

	double npm1 = -1.0 / (double)(*n);
	pow_real(&guess.d, &abs_a, &npm1);
	
	guess.d = ((*n + 1) * guess.d - (abs_a * (int_pow(&guess.d, *n + 1)))) / *n;

	if (!inv) {
		guess.d = *n / ((*n + 1) * guess.d - (abs_a * (int_pow(&guess.d, *n + 1))));
	}

	if ((int_pow(&guess.d, *n)) > abs_a) {
		guess.ull--;
	}

	if ((int_pow(&guess.d, *n)) < abs_a) {
		guess.ull++;
	}

	*root = guess.d;

	double power = int_pow(&guess.d, *n);

	if (power > abs_a) {
		guess.ull--;
	}
	else if (power == abs_a) {
		if (negative) {
			guess.d = -guess.d;
		}

		*root = guess.d;
		return;
	}

	double difference1 = abs_a - int_pow(&guess.d, *n);
	double difference2 = int_pow(root, *n) - abs_a;

	if (difference1 > difference2) {
		return;
	}

	if (negative) {
		*root = -guess.d;
	}
	else {
		*root = guess.d;
	}

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
		return osqrt(osqrt(a));
	}
	else if (n > 0) {
		double r;
		invroot(&r, &a, &n, false);
		return r;
	}
	else if (n < 0) {
		double r;
		n = -n;
		invroot(&r, &a, &n, true);
		return r;
	}
	else {
		return 0.0;
	}
}
