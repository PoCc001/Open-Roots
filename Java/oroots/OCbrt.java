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
		if (a == Double.POSITIVE_INFINITY || a == Double.NEGATIVE_INFINITY || a != a || a == 1.0d || a == 0.0d || a == -1.0d) {
			return a;
		}
		
		boolean negative = a < 0.0d;
		
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
		
		for (int i = 0; i < 6; i++) {
			guess = (2.0d * guess + (absA / (guess * guess))) / 3.0d;
		}
		
		double diff = (guess * guess * guess) - a;
		diff /= 3.0d * guess * guess;
		guess -= diff;
		
		if (negative) {
			guess = -guess;
		}
		
		return guess;
	}
	
	public static strictfp float cbrt (final float a) {
		if (a == Float.POSITIVE_INFINITY || a == Float.NEGATIVE_INFINITY || a != a || a == 1.0f || a == 0.0f || a == -1.0f) {
			return a;
		}
		
		boolean negative = a < 0.0f;
		
		float absA = negative ? -a : a;
		
		int intValue = Float.floatToRawIntBits(absA);
		
		int exponent = intValue >>> 23;
		
		boolean isSubNormal = exponent == 0;
		
		exponent -= 128;
		exponent /= 3;
		exponent += 128;
		
		if (isSubNormal) {
			int mantissa = longValue & FLOAT_MANTISSA_MASK;
			int subNormalExponent = Integer.numberOfLeadingZeros(mantissa) - 8;
			subNormalExponent /= 3;
			exponent -= subNormalExponent;
		}
		
		float guess = Float.intBitsToFloat(exponent << 23);
		
		for (int i = 0; i < 6; i++) {
			guess = (2.0f * guess + (absA / (guess * guess))) / 3.0f;
		}
		
		float diff = (guess * guess * guess) - a;
		diff /= 3.0f * guess * guess;
		guess -= diff;
		
		if (negative) {
			guess = -guess;
		}
		
		return guess;
	}
}
