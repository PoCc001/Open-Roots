/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

import static oroots.Masks;

public class OCbrt {	
	public static strictfp double cbrt (final double a) {
		if (a == Double.POSITIVE_INFINITY || a == Double.NEGATIVE_INFINITY || a != a || a == 1.0 || a == 0.0 || a == -1.0) {
			return a;
		}
		
		boolean negative = a < 0.0;
		
		double absA = negative ? -a : a;
		
		long longValue = Double.doubleToRawLongBits(absA);
		
		int exponent = (int)(longValue >>> 52);
		
		boolean isSubNormal = exponent == 0;
		
		exponent -= 1024;
		exponent /= 3;
		exponent += 1024;
		
		if (isSubNormal) {
			long mantissa = longValue & DOUBLE_MANTISSA_MASK;
			long subNormalExponent = Long.numberOfLeadingZeros(mantissa) - 11;
			subNormalExponent /= 3;
			exponent -= subNormalExponent;
		}
		
		double guess = Double.longBitsToDouble((long)(exponent) << 52);
		
		for (int i = 0; i < 5; i++) {
			guess = (2.0 * guess + (absA / (guess * guess))) / 3.0;
		}
		
		double diff = (guess * guess * guess) - a;
	   diff /= 3.0 * guess * guess;
	   guess -= diff;
		
		if (negative) {
			guess = -guess;
		}
		
		return guess;
	}
}
