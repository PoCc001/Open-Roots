/**
* Copyright Johannes Kloimböck 2020 - 2022.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef OROOTS_H
#define OROOTS_H

#include "osqrt.h"
#include "ocbrt.h"
#include <stdint.h>

#ifndef STRICT
#define STRICT 1
#endif

#ifndef CHECK_SPECIAL_CASES
#define CHECK_SPECIAL_CASES 1
#endif

#ifndef SUBNORMAL_NUMBERS
#define SUBNORMAL_NUMBERS 1
#endif

// Setting this to 1 could make the osqrt(f) and ocbrt(f) functions faster.
// However, the results may be less accurate!
#ifndef ONLY_USE_RECIP_ROOTS
#define ONLY_USE_RECIP_ROOTS 1
#endif

#ifndef USE_TABLES_FOR_FLOAT
#define USE_TABLES_FOR_FLOAT 0
#endif

#if STRICT == 1
typedef double corr_t;
typedef float corrf_t;
#else
typedef long double corr_t;
typedef double corrf_t;
#endif

typedef union {
	double d;
	uint64_t ull;
} double_ull;

typedef union {
	float f;
	uint32_t ul;
} float_ul;

#define DOUBLE_NAN 0x7fffffffffffffffULL
#define DOUBLE_INF 0x7ff0000000000000ULL

#define FLOAT_NAN 0x7fffffff
#define FLOAT_INF 0x7f800000

inline unsigned int leading_zeros_ui(const unsigned int *val) {
	unsigned int length = sizeof(*val) << 3;
	for (unsigned int i = length; i > 0; --i) {
		if (*val & (1 << (i - 1))) {
			return i;
		}
	}

	return 0;
}

extern double oroot(double, int);
extern float orootf(float, int);

#endif
