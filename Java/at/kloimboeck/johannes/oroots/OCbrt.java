/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package at.kloimboeck.johannes.oroots;

public class OCbrt {	
	public static strictfp double cbrt (final double a) {
		if (a == 0.0d) {
			return a;
		}
		
		long longValue = Double.doubleToRawLongBits(a);
		
		long sign = longValue & 0x8000000000000000L;
		
		longValue ^= sign;
		
		long exponent = longValue;
		
		boolean isSubNormal = (exponent & 0x7ff0000000000000L) == 0;
		
		exponent /= 3;
		exponent += 0x2a9f5cc62cb0f9e1L;
		int iterations = 3;
		
		if (isSubNormal) {
			iterations = 32;
		}
		
		long longGuess = exponent;
		longGuess |= sign;
		double guess = Double.longBitsToDouble(longGuess);
		
		for (int i = 0; i < iterations; i++) {
			guess = (2.0d * guess + (a / (guess * guess))) * 0.333333333333333333d;
		}
		
		double diff = (guess * guess * guess) - a;
		diff /= 3.0d * guess * guess;
		guess -= diff;
		
		return guess;
	}
	
	public static strictfp float cbrt (final float a) {
		if (a == 0.0f) {
			return a;
		}
		
		int intValue = Float.floatToRawIntBits(a);
		
		int sign = intValue & 0x80000000;
		
		intValue ^= sign;
		
		int exponent = intValue;
		
		boolean isSubNormal = (exponent & 0x7f800000) == 0;
		
		exponent /= 3;
		exponent += 0x2a501a5b;
		int iterations = 2;
		
		if (isSubNormal) {
			iterations = 31;
		}
		
		int intGuess = exponent;
		intGuess |= sign;
		float guess = Float.intBitsToFloat(intGuess);
		
		for (int i = 0; i < iterations; i++) {
			guess = (2.0f * guess + (a / (guess * guess))) * 0.333333333f;
		}
		
		double diff = (guess * guess * guess) - a;
		diff /= 3.0f * guess * guess;
		guess -= diff;
		
		return guess;
	}
}
