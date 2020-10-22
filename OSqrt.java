/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

import static oroots.Masks;

public class OSqrt {
	public static double sqrt(final double a) {
	   if (a < 0.0 || a == Double.NEGATIVE_INFINITY) {
		   return Double.NaN;
	   }
	   
	   if (a == 0.0d || a == 1.0d || a != a || a == Double.POSITIVE_INFINITY) {
		   return a;
	   }
	   
	   long longValue = Double.doubleToRawLongBits(a);
	   
	   int exponent = (int)(longValue >>> 52);
	   
	   boolean isSubNormal = exponent == 0;
	   
	   if ((exponent & DOUBLE_EXP_MASK_1) != 0) {
		   exponent &= DOUBLE_EXP_MASK_2;
		   exponent >>>= 1;
		   exponent |= DOUBLE_EXP_MASK_1;
	   } else {
		   int exponent2 = DOUBLE_EXP_MASK_1 - exponent;
		   exponent2 >>>= 1;
		   exponent = DOUBLE_EXP_MASK_1 - exponent2;
	   }
	   
	   if (isSubNormal) {
		   long mantissa = longValue & DOUBLE_MANTISSA_MASK;
		   long subNormalExponent = Long.numberOfLeadingZeros(mantissa) - 11;
		   subNormalExponent >>>= 1;
		   exponent -= subNormalExponent;
	   }
	   
	   double guess = Double.longBitsToDouble((long)(exponent) << 52);
	   
	   for (int i = 0; i <= 5; i++) {
			guess = (guess + (a / guess)) / 2;
	   }
	   
	   long longGuess;
	   
	   while ((guess * guess) > a) {
		   longGuess = Double.doubleToRawLongBits(guess);
		   longGuess--;
		   guess = Double.longBitsToDouble(longGuess);
	   }
	   
	   while ((guess * guess) < a) {
		   longGuess = Double.doubleToRawLongBits(guess);
		   longGuess++;
		   guess = Double.longBitsToDouble(longGuess);
	   }
	   
	   double guessLarger = guess;
	   
	   double sqr = guess * guess;
	   
	   if (sqr > a) {
		   longGuess = Double.doubleToRawLongBits(guess);
		   longGuess--;
		   guess = Double.longBitsToDouble(longGuess);
	   } else if (sqr == a) {
		   return guess;
	   }
	   
	   double difference1 = a - (guess * guess);
	   double difference2 = (guessLarger * guessLarger) - a;
	   
	   if (difference1 > difference2) {
		   guess = guessLarger;
	   }
	   
	   return guess;
    }
}
