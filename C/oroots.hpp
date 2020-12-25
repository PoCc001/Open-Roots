/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef OROOTS_HPP
#define OROOTS_HPP

extern "C" {
#include "oroots.h"
}

//extern "C" double asmsqrt(double);

//extern "C" double asmcbrt(double);


inline double cppsqrt(const double &x) noexcept {
	return csqrt(x);
}

inline double cppcbrt(const double &x) noexcept {
	return ccbrt(x);
}

inline double cpproot(const double &x, const int &n) noexcept {
	return croot(x, n);
}

#endif