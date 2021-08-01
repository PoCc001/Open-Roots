/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

public class OCbrt {	
	public static strictfp double cbrt (final double a) {
		if (a == 0.0d) {
			return a;
		}
		
		long longValue = Double.doubleToRawLongBits(a);
		
		long sign = longValue & 0x8000000000000000L;
		
		longValue &= 0x7fffffffffffffffL;
		
		int exponent = (int)(longValue >>> 52);
		
		boolean isSubNormal = exponent == 0;
		
		exponent /= 3;
		exponent += 683;
		
		if (isSubNormal) {
			long subNormalExponent = Long.numberOfLeadingZeros(longValue) - 11;
			subNormalExponent /= 3;
			exponent -= subNormalExponent;
		}
		
		long longGuess = (long)(exponent) << 52;
		longGuess |= sign;
		double guess = Double.longBitsToDouble(longGuess);
		
		for (int i = 0; i < 5; i++) {
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
		
		intValue &= 0x7fffffff;
		
		int exponent = intValue >>> 23;
		
		boolean isSubNormal = exponent == 0;
		
		exponent /= 3;
		exponent += 85;
		
		if (isSubNormal) {
			int subNormalExponent = Integer.numberOfLeadingZeros(intValue) - 8;
			subNormalExponent /= 3;
			exponent -= subNormalExponent;
		}
		
		int intGuess = exponent << 23;
		intGuess |= sign;
		float guess = Float.intBitsToFloat(intGuess);
		
		for (int i = 0; i < 4; i++) {
			guess = (2.0f * guess + (a / (guess * guess))) * 0.333333333f;
		}
		
		double diff = (guess * guess * guess) - a;
		diff /= 3.0f * guess * guess;
		guess -= diff;
		
		return guess;
	}
}
