/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

public class OSqrt {
	public static strictfp double sqrt(final double a) {
	   if (a < 0.0d) {
		   return Double.NaN;
	   } else if (a == 0.0d) {
		   return a;
	   }
	   
	   long longValue = Double.doubleToRawLongBits(a);
	   
	   int exponent = (int)(longValue >>> 52);
	   
	   boolean isSubNormal = exponent == 0;
	   
	   exponent >>= 1;
	   exponent += 512;
	   
	   if (isSubNormal) {
		   long subNormalExponent = Long.numberOfLeadingZeros(longValue) - 11;
		   subNormalExponent >>= 1;
		   exponent -= subNormalExponent;
	   }
	   
	   double guess = Double.longBitsToDouble((long)(exponent) << 52);
	   
	   for (int i = 0; i < 3; i++) {
			guess = (guess + (a / guess)) * 0.5d;
	   }
	   
	   double guesst2 = guess + (a / guess);
	   guess = guesst2 * 0.5d;
	   
	   double diff = (guess * guess) - a;
	   diff /= guesst2;
	   guess -= diff;
	   
	   return guess;
    }
	
	public static strictfp float sqrt(final float a) {
	   if (a < 0.0f) {
		   return Float.NaN;
	   } else if (a == 0.0f) {
		   return a;
	   }
	   
	   int intValue = Float.floatToRawIntBits(a);
	   
	   int exponent = intValue >>> 23;
	   
	   boolean isSubNormal = exponent == 0;
	   
	   exponent >>= 1;
	   exponent += 64;
	   
	   if (isSubNormal) {
		   int subNormalExponent = Integer.numberOfLeadingZeros(intValue) - 8;
		   subNormalExponent >>= 1;
		   exponent -= subNormalExponent;
	   }
	   
	   float guess = Float.intBitsToFloat(exponent << 23);
	   
	   for (int i = 0; i < 2; i++) {
			guess = (guess + (a / guess)) * 0.5f;
	   }
	   
	   float guesst2 = guess + (a / guess);
	   guess = guesst2 * 0.5f;
	   
	   float diff = (guess * guess) - a;
	   diff /= guesst2;
	   guess -= diff;
	   
	   return guess;
    }
}
