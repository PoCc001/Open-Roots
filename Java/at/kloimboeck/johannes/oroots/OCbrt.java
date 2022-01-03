/**
* Copyright Johannes Kloimböck 2020 - 2022.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package at.kloimboeck.johannes.oroots;

public class OCbrt {	
	public static double cbrt (final double a) {
		if (a == 0.0d) {
			return a;
		}
		
		long longValue = Double.doubleToRawLongBits(a);
		
		long sign = longValue & 0x8000000000000000L;
		
		longValue ^= sign;
		
		if (longValue == 0x7ff0000000000000L) {
			return a;
		}
		
		long exponent = longValue;
		
		boolean isSubNormal = (exponent & 0x7ff0000000000000L) == 0;
		
		exponent /= 3;
		exponent += 0x2a9f5cc62cb0f9e1L;
		int iterations = 4;
		
		if (isSubNormal) {
			iterations = 33;
		}
		
		long longGuess = exponent;
		longGuess |= sign;
		double guess = Double.longBitsToDouble(longGuess);
		
		double oneThirdA = a * 0.333333333333333333d;
		
		for (int i = 0; i < iterations; i++) {
			guess = guess * 0.6666666666666666667d + (oneThirdA / (guess * guess));
		}
		
		return guess;
	}
	
	public static float cbrt (final float a) {
		if (a == 0.0f) {
			return a;
		}
		
		int intValue = Float.floatToRawIntBits(a);
		
		int sign = intValue & 0x80000000;
		
		intValue ^= sign;
		
		if (intValue == 0x7f800000) {
			return a;
		}
		
		int exponent = intValue;
		
		boolean isSubNormal = (exponent & 0x7f800000) == 0;
		
		exponent /= 3;
		exponent += 0x2a501a5b;
		int iterations = 3;
		
		if (isSubNormal) {
			iterations = 32;
		}
		
		int intGuess = exponent;
		intGuess |= sign;
		float guess = Float.intBitsToFloat(intGuess);
		
		float oneThirdA = a * 0.333333333f;
		
		for (int i = 0; i < iterations; i++) {
			guess = guess * 0.666666667f + (oneThirdA / (guess * guess));
		}
		
		return guess;
	}
}
