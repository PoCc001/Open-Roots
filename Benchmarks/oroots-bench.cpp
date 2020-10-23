/**
* Copyright Johannes Kloimböck 2020.
* Distributed under the Boost Software License, Version 1.0.
* (See accompanying file LICENSE or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*/

#include "oroots.hpp"
#include <iostream>
#include <chrono>
#include <cmath>

constexpr unsigned int array_length = 10000;

int main() {
	std::cout << "Which function do you want to test?" << std::endl;
	std::cout << "- 1 -	Square root" << std::endl;
	std::cout << "- 2 -	Cube root" << std::endl;
	std::cout << "- 3 -	General root" << std::endl;
	int choice = 0;
	try {
		std::cin >> choice;
	}
	catch (...) {
		std::cout << "There's something wrong with the input!" << std::endl;
		std::cout << "Aborting!" << std::endl;
		system("PAUSE");
		return EXIT_SUCCESS;
	}

	double root_array_oroots[array_length];
	double root_array_std[array_length];
	printf("\n");
	std::cout << "Generating random numbers..." << std::endl;
	for (unsigned int i = 0; i < array_length; ++i) {
		root_array_oroots[i] = root_array_std[i] = rand();
	}
	std::cout << "Finished with generating random numbers!" << std::endl;
	printf("\n");

	if (choice == 1) {
		std::cout << "Calculate the square roots..." << std::endl;
		printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (unsigned int i = 0; i < array_length; ++i) {
			root_array_oroots[i] = cppsqrt(root_array_oroots[i]);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		begin = std::chrono::steady_clock::now();
		for (unsigned int i = 0; i < array_length; ++i) {
			root_array_oroots[i] = sqrt(root_array_oroots[i]);
		}
		end = std::chrono::steady_clock::now();
		double time2 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		double time_per_sqrt_1 = time1 / array_length;
		double time_per_sqrt_2 = time2 / array_length;

		std::cout << "Duration of " << array_length << " square roots (oroots): " << time1 << " sec." << std::endl;
		std::cout << "Duration of " << array_length << " square roots (std): " << time2 << " sec." << std::endl;
		std::cout << "Duration of one square root (oroots): " << time_per_sqrt_1 << " sec." << std::endl;
		std::cout << "Duration of one square root (std): " << time_per_sqrt_2 << " sec." << std::endl;
		if (time1 > time2) {
			double ratio = time1 / time2;
			printf("The std sqrt function is %3.3f times faster than the oroots sqrt function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			printf("The oroots sqrt function is %3.3f times faster than the std sqrt function.\n", ratio);
		}
	}
	else if (choice == 2) {
		std::cout << "Calculate the cube roots..." << std::endl;
		printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (unsigned int i = 0; i < array_length; ++i) {
			root_array_oroots[i] = cppcbrt(root_array_oroots[i]);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		begin = std::chrono::steady_clock::now();
		for (unsigned int i = 0; i < array_length; ++i) {
			root_array_oroots[i] = cbrt(root_array_oroots[i]);
		}
		end = std::chrono::steady_clock::now();
		double time2 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		double time_per_cbrt_1 = time1 / array_length;
		double time_per_cbrt_2 = time2 / array_length;

		std::cout << "Duration of " << array_length << " cube roots (oroots): " << time1 << " sec." << std::endl;
		std::cout << "Duration of " << array_length << " cube roots (std): " << time2 << " sec." << std::endl;
		std::cout << "Duration of one cube root (oroots): " << time_per_cbrt_1 << " sec." << std::endl;
		std::cout << "Duration of one cube root (std): " << time_per_cbrt_2 << " sec." << std::endl;
		if (time1 > time2) {
			double ratio = time1 / time2;
			printf("The std cbrt function is %3.3f times faster than the oroots cbrt function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			printf("The oroots cbrt function is %3.3f times faster than the std cbrt function.\n", ratio);
		}
	}
	else if (choice == 3) {
		std::cout << "Enter the degree of the roots!" << std::endl;
		int degree;
		try {
			std::cin >> degree;
		}
		catch (...) {
			std::cout << "There's something wrong with the input!" << std::endl;
			std::cout << "Aborting!" << std::endl;
			system("PAUSE");
			return EXIT_SUCCESS;
		}

		if (degree == 0) {
			std::cout << "Wrong input!" << std::endl;
			std::cout << "Aborting..." << std::endl;
			system("PAUSE");
			return EXIT_SUCCESS;
		}

		printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (unsigned int i = 0; i < array_length; ++i) {
			root_array_oroots[i] = cpproot(root_array_oroots[i], degree);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		double exponent = 1.0 / degree;

		begin = std::chrono::steady_clock::now();
		for (unsigned int i = 0; i < array_length; ++i) {
			root_array_oroots[i] = pow(root_array_oroots[i], exponent);
		}
		end = std::chrono::steady_clock::now();
		double time2 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		double time_per_root_1 = time1 / array_length;
		double time_per_root_2 = time2 / array_length;

		std::cout << "Duration of " << array_length << " general roots (oroots): " << time1 << " sec." << std::endl;
		std::cout << "Duration of " << array_length << " general roots (std): " << time2 << " sec." << std::endl;
		std::cout << "Duration of one general root (oroots): " << time_per_root_1 << " sec." << std::endl;
		std::cout << "Duration of one general root (std): " << time_per_root_2 << " sec." << std::endl;
		if (time1 > time2) {
			double ratio = time1 / time2;
			printf("The std pow function is %3.3f times faster than the oroots root function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			printf("The oroots root function is %3.3f times faster than the std pow function.\n", ratio);
		}
	}
	else {
		std::cout << "Wrong input!" << std::endl;
		std::cout << "Aborting..." << std::endl;
	}

	printf("\n");

	system("PAUSE");
	return EXIT_SUCCESS;
}