/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

import static oroots.Masks;

public class OCbrt {	
	public static double cbrt (final double a) {
		if (a == Double.POSITIVE_INFINITY || a == Double.NEGATIVE_INFINITY || a != a || a == 1.0 || a == 0.0 || a == -1.0) {
			return a;
		}
		
		boolean negative = a < 0.0;
		
		double absA = negative ? -a : a;
		
		long longValue = Double.doubleToRawLongBits(absA);
		
		int exponent = (int)(longValue >>> 52);
		
		boolean isSubNormal = exponent == 0;
		
		if ((exponent & DOUBLE_EXP_MASK_1) != 0) {
			exponent &= DOUBLE_EXP_MASK_2;
			exponent /= 3;
			exponent |= DOUBLE_EXP_MASK_1;
		} else {
			int exponent2 = DOUBLE_EXP_MASK_1 - exponent;
			exponent2 /= 3;
			exponent = DOUBLE_EXP_MASK_1 - exponent2;
		}
		
		if (isSubNormal) {
			long mantissa = longValue & DOUBLE_MANTISSA_MASK;
			long subNormalExponent = Long.numberOfLeadingZeros(mantissa) - 11;
			subNormalExponent /= 3;
			exponent -= subNormalExponent;
		}
		
		double guess = Double.longBitsToDouble((long)(exponent) << 52);
		
		for (int i = 0; i < 10; i++) {
			guess = (2.0 * guess + (absA / (guess * guess))) / 3.0;
		}
		
		long longGuess;
		
		while ((guess * guess * guess) > absA) {
			longGuess = Double.doubleToRawLongBits(guess);
			longGuess--;
			guess = Double.longBitsToDouble(longGuess);
		}
		
		while ((guess * guess * guess) < absA) {
			longGuess = Double.doubleToRawLongBits(guess);
			longGuess++;
			guess = Double.longBitsToDouble(longGuess);
		}
		
		double guessLarger = guess;
		
		double cube = guess * guess * guess;
		
		if (cube > absA) {
			longGuess = Double.doubleToRawLongBits(guess);
			longGuess--;
			guess = Double.longBitsToDouble(longGuess);
		} else if (cube == absA) {
			if (negative) {
				guess = -guess;
			}
			
			return guess;
		}
		
		double difference1 = absA - (guess * guess * guess);
		double difference2 = (guessLarger * guessLarger * guessLarger) - absA;
		
		if (difference1 > difference2) {
			guess = guessLarger;
		}
		
		if (negative) {
			guess = -guess;
		}
		
		return guess;
	}
}
