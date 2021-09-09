/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package at.kloimboeck.johannes.oroots;

public class OSqrt {	
	public static double rsqrt(final double a) {
		if (a == 0.0d) {
			return a == -0.0 ? Double.NEGATIVE_INFINITY : Double.POSITIVE_INFINITY;
		}
		
		long longValue = Double.doubleToRawLongBits(a);
		
		long exponent = longValue;
		
		boolean isSubNormal = (exponent & 0x7ff0000000000000L) == 0;
		
		exponent = 0xbfcc409b00000000L - exponent;
		exponent >>>= 1;
		
		int iterations = 4;
		
		if (isSubNormal) {
			iterations = 50;
		}
		
		double guess = Double.longBitsToDouble(exponent);
		double halfA = a * 0.5d;
		
		for (int i = 0; i < iterations; ++i) {
			guess = guess + (guess * (0.5d - (halfA * guess * guess)));
		}
		
		return a == Double.POSITIVE_INFINITY ? 0.0 : guess;
	}
	
	public static double sqrt(final double a) {
	   if (a == 0.0d) {
		   return a;
	   }
	   
	   long longValue = Double.doubleToRawLongBits(a);
	   
	   long exponent = longValue;
	   
	   boolean isSubNormal = (exponent & 0x7ff0000000000000L) == 0;
	   
	   exponent >>>= 1;
	   exponent += 0x1ff62ddf00000000L;
	   int iterations = 4;
	   
	   if (isSubNormal) {
		   iterations = 32;
	   }
	   
	   double guess = Double.longBitsToDouble(exponent);
	   
	   for (int i = 0; i < iterations; i++) {
			guess = (guess + (a / guess)) * 0.5d;
	   }
	   
	   return a == Double.POSITIVE_INFINITY ? a : guess;
    }
	
	public static float rsqrt(final float a) {
		if (a == 0.0f) {
			return a == -0.0f ? Float.NEGATIVE_INFINITY : Float.POSITIVE_INFINITY;
		}
		
		int intValue = Float.floatToRawIntBits(a);
		
		int exponent = intValue;
		
		boolean isSubNormal = (exponent & 0x7f800000) == 0;
		
		exponent = 0xbe700000 - exponent;
		exponent >>>= 1;
	   	int iterations = 3;
		
		if (isSubNormal) {
			iterations = 45;
		}
		
		float guess = Float.intBitsToFloat(exponent);
		float halfA = a * 0.5f;
		
		for (int i = 0; i < iterations; ++i) {
			guess = guess + (guess * (0.5f - (halfA * guess * guess)));
		}
		
		return a == Float.POSITIVE_INFINITY ? 0.0f : guess;
	}
	
	public static float sqrt(final float a) {
	   if (a == 0.0f) {
		   return a;
	   }
	   
	   int intValue = Float.floatToRawIntBits(a);
	   
	   int exponent = intValue;
	   
	   boolean isSubNormal = (exponent & 0x7f800000) == 0;
	   
	   exponent >>>= 1;
	   exponent += 0x1f94da6d;
	   int iterations = 3;
	   
	   if (isSubNormal) {
		   iterations = 35;
	   }
	   
	   float guess = Float.intBitsToFloat(exponent);
	   
	   for (int i = 0; i < iterations; i++) {
			guess = (guess + (a / guess)) * 0.5f;
	   }
	   
	   return a == Float.POSITIVE_INFINITY ? a : guess;
    }
	
	public static void main (String [] args) {
		System.out.println(rsqrt(Double.NaN));
	}
}
