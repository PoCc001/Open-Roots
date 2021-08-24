/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef OROOTS_HPP
#define OROOTS_HPP

extern "C" {
#include "oroots.h"
}

inline float orsqrt(const float& x) noexcept {
	return orsqrtf(x);
}

inline float osqrt(const float &x) noexcept {
	return osqrtf(x);
}

inline float ocbrt(const float &x) noexcept {
	return ocbrtf(x);
}

inline float oroot(const float &x, const int &n) noexcept {
	return orootf(x, n);
}

#endif
