/**
* Copyright Johannes Kloimböck 2020 - 2021.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

package oroots;

public strictfp class ORoots {
	private final static double LOG_2 = 0.6931471805599453;
	private final static double E = 2.718281828459045;
	private final static int DOUBLE_EXP_MASK_1 = 0b10000000000;
	private final static int DOUBLE_EXP_MASK_2 = 0b1111111111;
	private final static int DOUBLE_EXP_MASK_3 = 0b11111111111;
	
	public static double sqrt (final double a) {
		return OSqrt.sqrt(a);
	}
	
	public static double rsqrt (final double a) {
		return OSqrt.rsqrt(a);
	}
	
	public static float sqrt (final float a) {
		return OSqrt.sqrt(a);
	}
	
	public static float rsqrt (final float a) {
		return OSqrt.rsqrt(a);
	}
	
	public static double cbrt (final double a) {
		return OCbrt.cbrt(a);
	}
	
	public static float cbrt (final float a) {
		return OCbrt.cbrt(a);
	}
	
	public static double root (final double a, int n) {
		if (n == 2) {
			return sqrt(a);
		} else if (n == 3) {
			return cbrt(a);
		} else if (n == 1) {
			return a;
		} else if (n == -1) {
			return 1.0 / a;
		}
		
		if (n > 0) {
			return invroot(a, n);
		} else if (n < 0) {
			return 1.0 / invroot(a, -n);
		} else {
			return Double.NaN;
		}
	}
	
	public static float root (final float a, int n) {
		return (float)(root((double)(a), n));
	}
	
	private static double invroot (final double a, final int mn) {
		if (a < 0.0 && (mn & 1) == 0) {
			return Double.NaN;
		}
		
		if (a == Double.POSITIVE_INFINITY || a == Double.NEGATIVE_INFINITY || a != a || a == 1.0 || a == 0.0 || a == -1.0) {
			return a;
		}
		
		boolean negative = a < 0.0;
		
		double absA = negative ? -a : a;
		
		double guess = 1.0;
		
		if (mn < 5 && mn > -5) {
			long longValue = Double.doubleToRawLongBits(absA);
			
			int exponent = (int)(longValue >>> 52);
			
			boolean isSubNormal = exponent == 0;
			
			exponent -= 1024;
			exponent /= mn;
			exponent += 1024;
			
			if (isSubNormal) {
				long subNormalExponent = Long.numberOfLeadingZeros(longValue) - 11;
				subNormalExponent /= mn;
				exponent -= subNormalExponent;
			}
			
			guess = 1.0 / Double.longBitsToDouble((long)(exponent) << 52);
		} else {
			guess = power(absA, -1.0 / (double)(mn));
		}
		
		guess = ((mn + 1) * guess - (absA * (intPower(guess, mn + 1)))) / mn;
		guess = mn / ((mn + 1) * guess - (absA * (intPower(guess, mn + 1))));
		
		long longGuess;
		
		while ((intPower(guess, mn)) > absA) {
			longGuess = Double.doubleToRawLongBits(guess);
			longGuess--;
			guess = Double.longBitsToDouble(longGuess);
		}
		
		while ((intPower(guess, mn)) < absA) {
			longGuess = Double.doubleToRawLongBits(guess);
			longGuess++;
			guess = Double.longBitsToDouble(longGuess);
		}
		
		double guessLarger = guess;
		
		double power = intPower(guess, mn);
		
		if (power > absA) {
			longGuess = Double.doubleToRawLongBits(guess);
			longGuess--;
			guess = Double.longBitsToDouble(longGuess);
		} else if (power == absA) {
			if (negative) {
				guess = -guess;
			}
			
			return guess;
		}
		
		double difference1 = absA - intPower(guess, mn);
		double difference2 = intPower(guessLarger, mn) - absA;
		
		if (difference1 > difference2) {
			guess = guessLarger;
		}
		
		if (negative) {
			guess = -guess;
		}
		
		return guess;
	}
	
	private static double intPower (final double a, int p) {
		double pow = (p & 1) == 0 ? 1.0 : a;
		p >>>= 1;
		int length = 32 - Integer.numberOfLeadingZeros(p);
		double sqr = a * a;
		for (int i = 0; i < length; i++) {
			if ((p & 1) != 0) {
				pow *= sqr;
			}
			
			sqr *= sqr;
			p >>>= 1;
		}
		
		return pow;
	}
	
	private static double expSmall (final double a) {
		double sum = 1.0;
		double term = 1.0;
		double k = 1.0;
		while (term > 1E-10 || term < -1E-10) {
			term *= a;
			term /= k;
			sum += term;
			k += 1.0;
		}
		
		return sum;
	}
	
	private static double power (double a, double b) {
		long intPart = intPart(b);
		double rationalPart = b - intPart;
		double result = expSmall(rationalPart * log(a));
		return result * intPower(E, (int)intPart);
	}
	
	private static double log (double a) {
		long aLong = Double.doubleToRawLongBits(a);
		int exponent = (int)(aLong >>> 52) & DOUBLE_EXP_MASK_3;
		int powerOfTwo = exponent - DOUBLE_EXP_MASK_1;
		aLong &= ((1L << 53) - 1);
		aLong |= (1L << 62);
		a = Double.longBitsToDouble(aLong);
		double sum = (a - 1.0) / (a + 1.0);
		double term = sum;
		double j = 1.0;
		for (int i = 0; i < 25; i++) {
			term *= j * (a - 1.0) * (a - 1.0);
			j += 2.0;
			term /= (j * (a + 1.0) * (a + 1.0));
			sum += term;
		}
		
		return sum * 2.0 + powerOfTwo * LOG_2;
	}
	
	private static long intPart (double a) {
		if (a < 1.0 && a > -1.0) {
			return 0;
		} else {
			long longVal = Double.doubleToRawLongBits(a);
			int exponent = ((int)(longVal >>> 52) + 2) & DOUBLE_EXP_MASK_2;
			long mantissa = longVal & ((1L << 53) - 1);
			long intP = mantissa >>> (52 - exponent);
			intP |= (1L << exponent);
			return intP;
		}
	}
}
