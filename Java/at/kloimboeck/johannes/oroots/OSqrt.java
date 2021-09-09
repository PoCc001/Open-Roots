/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package at.kloimboeck.johannes.oroots;

public class OSqrt {
	private static final double RSQRT_1L = Double.longBitsToDouble(0x6180000000000000L);
	private static final float RSQRT_1 = Float.intBitsToFloat(0x653504f3);
	
	public static strictfp double rsqrt(final double a) {
		if (a < 0.0d) {
			return Double.NaN;
		} else if (a == 0.0d) {
			return Double.POSITIVE_INFINITY;
		}
		
		long longValue = Double.doubleToRawLongBits(a);
		
		if (longValue == 1L) {
			return RSQRT_1L;
		}
		
		long exponent = longValue;
		
		boolean isSubNormal = (exponent & 0x7ff0000000000000L) == 0;
		
		exponent = 0xbfcc409b00000000L - exponent;
		exponent >>>= 1;
		
		int iterations = 4;
		
		if (isSubNormal) {
			iterations = 31;
		}
		
		double guess = Double.longBitsToDouble(exponent);
		double halfA = a * 0.5d;
		
		for (int i = 0; i < iterations; ++i) {
			guess = guess + (guess * (0.5d - (halfA * guess * guess)));
		}
		
		return guess;
	}
	
	public static strictfp double sqrt(final double a) {
	   if (a < 0.0d) {
		   return Double.NaN;
	   } else if (a == 0.0d) {
		   return a;
	   }
	   
	   long longValue = Double.doubleToRawLongBits(a);
	   
	   long exponent = longValue;
	   
	   boolean isSubNormal = (exponent & 0x7ff0000000000000L) == 0;
	   
	   exponent >>>= 1;
	   exponent += 0x1ff62ddf00000000L;
	   int iterations = 2;
	   
	   if (isSubNormal) {
		   iterations = 30;
	   }
	   
	   double guess = Double.longBitsToDouble(exponent);
	   
	   for (int i = 0; i < iterations; i++) {
			guess = (guess + (a / guess)) * 0.5d;
	   }
	   
	   double guesst2 = guess + (a / guess);
	   guess = guesst2 * 0.5d;
	   
	   double diff = (guess * guess) - a;
	   diff /= guesst2;
	   guess -= diff;
	   
	   return guess;
    }
	
	public static strictfp float rsqrt(final float a) {
		if (a < 0.0f) {
			return Float.NaN;
		} else if (a == 0.0f) {
			return Float.POSITIVE_INFINITY;
		}
		
		int intValue = Float.floatToRawIntBits(a);
		
		if (intValue == 1) {
			return RSQRT_1;
		}
		
		int exponent = intValue;
		
		boolean isSubNormal = (exponent & 0x7f800000) == 0;
		
		exponent = 0xbe700000 - exponent;
		exponent >>>= 1;
	   	int iterations = 3;
		
		if (isSubNormal) {
			iterations = 31;
		}
		
		float guess = Float.intBitsToFloat(exponent);
		float halfA = a * 0.5f;
		
		for (int i = 0; i < iterations; ++i) {
			guess = guess + (guess * (0.5f - (halfA * guess * guess)));
		}
		
		return guess;
	}
	
	public static strictfp float sqrt(final float a) {
	   if (a < 0.0f) {
		   return Float.NaN;
	   } else if (a == 0.0f) {
		   return a;
	   }
	   
	   int intValue = Float.floatToRawIntBits(a);
	   
	   int exponent = intValue;
	   
	   boolean isSubNormal = (exponent & 0x7f800000) == 0;
	   
	   exponent >>>= 1;
	   exponent += 0x1f94da6d;
	   int iterations = 1;
	   
	   if (isSubNormal) {
		   iterations = 33;
	   }
	   
	   float guess = Float.intBitsToFloat(exponent);
	   
	   for (int i = 0; i < iterations; i++) {
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
