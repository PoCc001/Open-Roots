/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

import oroots.*;
import java.util.Scanner;

public class ORootsBench {
	private static final int array_length = 10000;
	
	public static void main (String [] args) {
		System.out.println("Which function do you want to test?");
		System.out.println("- 1 -	Square root");
		System.out.println("- 2 -	Cube root");
		System.out.println("- 3 -	General root");
		Scanner scanner = new Scanner (System.in);
		int choice = 0;
		try {
			choice = scanner.nextInt();
		} catch (NumberFormatException nfe) {
			System.out.println("There's something wrong with the input!");
			System.out.println("Aborting!");
			return;
		}
		
		double []root_array_oroots = new double[array_length];
		double []root_array_std = new double[array_length];
		System.out.println();
		System.out.println("Generating random numbers...");
		for (int i = 0; i < array_length; i++) {
			root_array_oroots[i] = root_array_std[i] = (int)(Math.random() * 100000);
		}
		System.out.println("Finished with generating random numbers!");
		System.out.println();
		
		if (choice == 1) {
			System.out.println("Calculate the square roots...");
			System.out.println();
			
			long s = System.nanoTime();
			for (int i = 0; i < array_length; i++) {
				root_array_oroots[i] = ORoots.sqrt(root_array_oroots[i]);
			}
			long e = System.nanoTime() - s;
			double time1 = e / 1E9;
			
			s = System.nanoTime();
			for (int i = 0; i < array_length; i++) {
				root_array_std[i] = Math.sqrt(root_array_std[i]);
			}
			e = System.nanoTime() - s;
			double time2 = e / 1E9;
			
			double time_per_sqrt_1 = time1 / array_length;
			double time_per_sqrt_2 = time2 / array_length;
			
			System.out.println("Duration of " + array_length + " square roots (oroots): " + time1 + " sec.");
			System.out.println("Duration of " + array_length + " square roots (java.lang.Math): " + time2 + " sec.");
			System.out.println("Duration of one square root (oroots): " + time_per_sqrt_1 + " sec.");
			System.out.println("Duration of one square root (java.lang.Math) " + time_per_sqrt_2 + " sec.");
			if (time1 > time2) {
				double ratio = time1 / time2;
				System.out.printf("The java.lang.Math sqrt function is %3.3f times faster than the oroots sqrt function.\n", ratio);
			} else {
				double ratio = time2 / time1;
				System.out.printf("The oroots sqrt function is %3.3f times faster than the java.lang.Math sqrt function.\n", ratio);
			}
			
		} else if (choice == 2) {
			System.out.println("Calculate the cube roots...");
			System.out.println();
			
			long s = System.nanoTime();
			for (int i = 0; i < array_length; i++) {
				root_array_oroots[i] = ORoots.cbrt(root_array_oroots[i]);
			}
			long e = System.nanoTime() - s;
			double time1 = e / 1E9;
			
			s = System.nanoTime();
			for (int i = 0; i < array_length; i++) {
				root_array_std[i] = Math.cbrt(root_array_std[i]);
			}
			e = System.nanoTime() - s;
			double time2 = e / 1E9;
			
			double time_per_cbrt_1 = time1 / array_length;
			double time_per_cbrt_2 = time2 / array_length;
			
			System.out.println("Duration of " + array_length + " cube roots (oroots): " + time1 + " sec.");
			System.out.println("Duration of " + array_length + " cube roots (java.lang.Math): " + time2 + " sec.");
			System.out.println("Duration of one cube root (oroots): " + time_per_cbrt_1 + " sec.");
			System.out.println("Duration of one cube root (java.lang.Math): " + time_per_cbrt_2 + " sec.");
			if (time1 > time2) {
				double ratio = time1 / time2;
				System.out.printf("The java.lang.Math cbrt function is %3.3f times faster than the oroots cbrt function.\n", ratio);
			} else {
				double ratio = time2 / time1;
				System.out.printf("The oroots cbrt function is %3.3f times faster than the java.lang.Math cbrt function.\n", ratio);
			}
		} else if (choice == 3) {
			System.out.println("Enter the degree of the roots!");
			int degree = 0;
			try {
				degree = scanner.nextInt();
			} catch (NumberFormatException nfe) {
				System.out.println("There's something wrong with the input!");
				System.out.println("Aborting!");
				return;
			}
			
			if (degree == 0) {
				System.out.println("Wrong input!");
				System.out.println("Aborting...");
				return;
			}
			
			System.out.println();
			
			long s = System.nanoTime();
			for (int i = 0; i < array_length; i++) {
				root_array_oroots[i] = ORoots.root(root_array_oroots[i], degree);
			}
			long e = System.nanoTime() - s;
			double time1 = e / 1E9;
			
			double exponent = 1.0 / degree;
			
			s = System.nanoTime();
			for (int i = 0; i < array_length; i++) {
				root_array_std[i] = Math.pow(root_array_std[i], exponent);
			}
			e = System.nanoTime() - s;
			double time2 = e / 1E9;
			
			double time_per_root_1 = time1 / array_length;
			double time_per_root_2 = time2 / array_length;
			
			System.out.println("Duration of " + array_length + " general roots (oroots): " + time1 + " sec.");
			System.out.println("Duration of " + array_length + " general roots (java.lang.Math): " + time2 + " sec.");
			System.out.println("Duration of one general root (oroots): " + time_per_root_1 + " sec.");
			System.out.println("Duration of one general root (java.lang.Math): " + time_per_root_2 + " sec.");
			if (time1 > time2) {
				double ratio = time1 / time2;
				System.out.printf("The java.lang.Math pow function is %3.3f times faster than the oroots root function.\n", ratio);
			} else {
				double ratio = time2 / time1;
				System.out.printf("The oroots root function is %3.3f times faster than the java.lang.Math pow function.\n", ratio);
			}
		} else {
			System.out.println("Wrong input!");
			System.out.println("Aborting...");
		}
		
		System.out.println();
	}
}