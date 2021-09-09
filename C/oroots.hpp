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

inline float orcbrt(const float& x) noexcept {
	return orcbrtf(x);
}

inline float ocbrt(const float &x) noexcept {
	return ocbrtf(x);
}

inline float oroot(const float &x, const int &n) noexcept {
	return orootf(x, n);
}

namespace oroots {
	inline double rsqrt(const double& x) noexcept {
		return orsqrt(x);
	}

	inline float rsqrt(const float& x) noexcept {
		return orsqrtf(x);
	}

	inline double sqrt(const double& x) noexcept {
		return osqrt(x);
	}

	inline float sqrt(const float& x) noexcept {
		return osqrtf(x);
	}

	inline double rcbrt(const double& x) noexcept {
		return orcbrt(x);
	}

	inline float rcbrt(const float& x) noexcept {
		return orcbrtf(x);
	}

	inline double cbrt(const double& x) noexcept {
		return ocbrt(x);
	}

	inline float cbrt(const float& x) noexcept {
		return ocbrtf(x);
	}

	inline double root(const double& x, const int& n) noexcept {
		return oroot(x, n);
	}

	inline float root(const float& x, const int& n) noexcept {
		return orootf(x, n);
	}
}

#endif
