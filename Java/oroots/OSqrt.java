/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

import static oroots.Masks;

public class OSqrt {
	public static strictfp double sqrt(final double a) {
	   if (a < 0.0 || a == Double.NEGATIVE_INFINITY) {
		   return Double.NaN;
	   }
	   
	   if (a == 0.0d || a == 1.0d || a != a || a == Double.POSITIVE_INFINITY) {
		   return a;
	   }
	   
	   long longValue = Double.doubleToRawLongBits(a);
	   
	   int exponent = (int)(longValue >>> 52);
	   
	   boolean isSubNormal = exponent == 0;
	   
	   exponent -= 1023;
	   exponent >>= 1;
	   exponent += 1023;
	   
	   if (isSubNormal) {
		   long mantissa = longValue & DOUBLE_MANTISSA_MASK;
		   long subNormalExponent = Long.numberOfLeadingZeros(mantissa) - 11;
		   subNormalExponent >>= 1;
		   exponent -= subNormalExponent;
	   }
	   
	   double guess = Double.longBitsToDouble((long)(exponent) << 52);
	   
	   for (int i = 0; i < 5; i++) {
			guess = (guess + (a / guess)) / 2;
	   }
	   
	   return guess;
    }
}
