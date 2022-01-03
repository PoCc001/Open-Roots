#include "oroots.hpp"
#include <iostream>
#include <chrono>
#include <cmath>

#pragma warning(disable : 4996)

constexpr std::size_t array_length = 10'000'000;

int bench64() {
	std::cout << "64-BIT TEST" << std::endl;
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

	double* rand_array = new double[array_length];
	double* root_array_oroots = new double[array_length];
	double* root_array_std = new double[array_length];

	std::printf("\n");
	std::cout << "Generating random numbers..." << std::endl;
	for (unsigned int i = 0; i < array_length; ++i) {
		rand_array[i] = rand();

		if (i % 1000 == 0) {
			rand_array[i] = -rand_array[i];
		}
	}
	std::cout << "Finished with generating random numbers!" << std::endl;
	std::printf("\n");

	std::string rootString;

	if (choice == 1) {
		rootString = "sqrt";
		std::cout << "Calculate the square roots..." << std::endl;
		std::printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_oroots[i] = oroots::sqrt(rand_array[i]);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_std[i] = std::sqrt(rand_array[i]);
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
			std::printf("The std sqrt function is %3.3f times faster than the oroots sqrt function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			std::printf("The oroots sqrt function is %3.3f times faster than the std sqrt function.\n", ratio);
		}
	}
	else if (choice == 2) {
		rootString = "cbrt";
		std::cout << "Calculate the cube roots..." << std::endl;
		std::printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_oroots[i] = oroots::cbrt(rand_array[i]);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_std[i] = std::cbrt(rand_array[i]);
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
			std::printf("The std cbrt function is %3.3f times faster than the oroots cbrt function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			std::printf("The oroots cbrt function is %3.3f times faster than the std cbrt function.\n", ratio);
		}
	}
	else if (choice == 3) {
		rootString = "root";
		std::cout << "Enter the degree of the roots!" << std::endl;
		int degree;
		try {
			std::cin >> degree;
		}
		catch (...) {
			std::cout << "There's something wrong with the input!" << std::endl;
			std::cout << "Aborting!" << std::endl;
			std::system("PAUSE");
			return EXIT_SUCCESS;
		}

		if (degree == 0) {
			std::cout << "Wrong input!" << std::endl;
			std::cout << "Aborting..." << std::endl;
			std::system("PAUSE");
			return EXIT_SUCCESS;
		}

		std::printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_oroots[i] = oroots::root(rand_array[i], degree);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		double exponent = 1.0 / degree;

		begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_std[i] = std::pow(rand_array[i], exponent);
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
			std::printf("The std pow function is %3.3f times faster than the oroots root function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			std::printf("The oroots root function is %3.3f times faster than the std pow function.\n", ratio);
		}
	}
	else {
		std::cout << "Wrong input!" << std::endl;
		std::cout << "Aborting..." << std::endl;
		std::system("PAUSE");
		return EXIT_SUCCESS;
	}

	std::printf("\n");

	std::cout << "Writing roots and other info to file \"oroots-bench_64.txt\"" << std::endl;
	FILE *oroots_bench_txt = fopen("oroots-bench_64.txt", "w");
	
	if (oroots_bench_txt == nullptr) {
		std::cout << "Cannot create/open file!" << std::endl;
		std::cout << "Aborting..." << std::endl;
		delete[] rand_array;
		delete[] root_array_oroots;
		delete[] root_array_std;
		std::system("PAUSE");
		return EXIT_SUCCESS;
	}
	
	std::fprintf(oroots_bench_txt, "input\t\t|\t\tstd\t\t|\t\toroots\t\t|\tdifference (bits)\n");
	long long* diff_array = new long long[array_length];
	double_ull std;
	double_ull oroots;
	for (std::size_t i = 0; i < array_length; ++i) {
		std.d = root_array_std[i];
		oroots.d = root_array_oroots[i];
		diff_array[i] = (long long)(std.ull) - (long long)(oroots.ull);
	}

	for (std::size_t i = 0; i < array_length; ++i) {
		std::fprintf(oroots_bench_txt, "%s(%.1f):\t|\t%20.16g\t|\t%20.16g\t|\t%lli\n", rootString.c_str(), rand_array[i], root_array_std[i], root_array_oroots[i], diff_array[i]);
	}

	fclose(oroots_bench_txt);

	std::size_t counter = 0;
	unsigned long long max_diff = 0;
	double signed_avg_diff = 0.0;
	double unsigned_avg_diff = 0.0;
	for (std::size_t i = 0; i < array_length; ++i) {
		if (diff_array[i] != 0) {
			if (max_diff < std::abs(diff_array[i])) {
				max_diff = std::abs(diff_array[i]);
			}
			signed_avg_diff += (double)(diff_array[i]);
			unsigned_avg_diff += abs((double)(diff_array[i]));
			counter++;
		}
	}

	signed_avg_diff /= (double)(array_length);
	unsigned_avg_diff /= (double)(array_length);

	double ratio = 100.0 * counter / array_length;

	std::cout << ratio << "% of computed roots don't exactly match." << std::endl;
	std::cout << "Greatest bit difference: " << max_diff << " bits" << std::endl;
	std::cout << "Average bit difference: " << signed_avg_diff << " bits" << std::endl;
	std::cout << "Average absolute bit difference: " << unsigned_avg_diff << " bits" << std::endl;

	std::printf("\n");

	delete[] rand_array;
	delete[] root_array_oroots;
	delete[] root_array_std;
	delete[] diff_array;

	system("PAUSE");
	return EXIT_SUCCESS;
}

int bench32() {
	std::cout << "32-BIT TEST" << std::endl;
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

	float* rand_array = new float[array_length];
	float* root_array_oroots = new float[array_length];
	float* root_array_std = new float[array_length];

	std::printf("\n");
	std::cout << "Generating random numbers..." << std::endl;
	for (std::size_t i = 0; i < array_length; ++i) {
		rand_array[i] = (float)(rand());

		if (i % 1000 == 0) {
			rand_array[i] = -rand_array[i];
		}
	}
	std::cout << "Finished with generating random numbers!" << std::endl;
	std::printf("\n");

	std::string rootString;

	if (choice == 1) {
		rootString = "sqrt";
		std::cout << "Calculate the square roots..." << std::endl;
		std::printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_oroots[i] = oroots::sqrt(rand_array[i]);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_std[i] = std::sqrt(rand_array[i]);
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
			std::printf("The std sqrt function is %3.3f times faster than the oroots sqrt function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			std::printf("The oroots sqrt function is %3.3f times faster than the std sqrt function.\n", ratio);
		}
	}
	else if (choice == 2) {
		rootString = "cbrt";
		std::cout << "Calculate the cube roots..." << std::endl;
		std::printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_oroots[i] = oroots::cbrt(rand_array[i]);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_std[i] = std::cbrt(rand_array[i]);
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
			std::printf("The std cbrt function is %3.3f times faster than the oroots cbrt function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			std::printf("The oroots cbrt function is %3.3f times faster than the std cbrt function.\n", ratio);
		}
	}
	else if (choice == 3) {
		rootString = "root";
		std::cout << "Enter the degree of the roots!" << std::endl;
		int degree;
		try {
			std::cin >> degree;
		}
		catch (...) {
			std::cout << "There's something wrong with the input!" << std::endl;
			std::cout << "Aborting!" << std::endl;
			std::system("PAUSE");
			return EXIT_SUCCESS;
		}

		if (degree == 0) {
			std::cout << "Wrong input!" << std::endl;
			std::cout << "Aborting..." << std::endl;
			std::system("PAUSE");
			return EXIT_SUCCESS;
		}

		std::printf("\n");

		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_oroots[i] = oroots::root(rand_array[i], degree);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double time1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() / 1E9;

		float exponent = 1.0f / degree;

		begin = std::chrono::steady_clock::now();
		for (std::size_t i = 0; i < array_length; ++i) {
			root_array_std[i] = std::pow(rand_array[i], exponent);
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
			std::printf("The std pow function is %3.3f times faster than the oroots root function.\n", ratio);
		}
		else {
			double ratio = time2 / time1;
			std::printf("The oroots root function is %3.3f times faster than the std pow function.\n", ratio);
		}
	}
	else {
		std::cout << "Wrong input!" << std::endl;
		std::cout << "Aborting..." << std::endl;
		std::system("PAUSE");
		return EXIT_SUCCESS;
	}

	std::printf("\n");

	std::cout << "Writing roots and other info to file \"oroots-bench_32.txt\"" << std::endl;
	FILE* oroots_bench_txt = fopen("oroots-bench_32.txt", "w");
	
	if (oroots_bench_txt == nullptr) {
		std::cout << "Cannot create/open file!" << std::endl;
		std::cout << "Aborting..." << std::endl;
		delete[] rand_array;
		delete[] root_array_oroots;
		delete[] root_array_std;
		std::system("PAUSE");
		return EXIT_SUCCESS;
	}
	
	std::fprintf(oroots_bench_txt, "input\t\t|\tstd\t\t|\toroots\t\t|\tdifference (bits)\n");
	long* diff_array = new long[array_length];
	float_ul std;
	float_ul oroots;
	for (std::size_t i = 0; i < array_length; ++i) {
		std.f = root_array_std[i];
		oroots.f = root_array_oroots[i];
		diff_array[i] = (long)(std.ul) - (long)(oroots.ul);
	}

	for (std::size_t i = 0; i < array_length; ++i) {
		std::fprintf(oroots_bench_txt, "%s(%.1f):\t|%15.7g\t|%15.7g\t|\t%li\n", rootString.c_str(), rand_array[i], root_array_std[i], root_array_oroots[i], diff_array[i]);
	}

	fclose(oroots_bench_txt);

	std::size_t counter = 0;
	unsigned long max_diff = 0;
	double signed_avg_diff = 0.0;
	double unsigned_avg_diff = 0.0;

	for (std::size_t i = 0; i < array_length; ++i) {
		if (diff_array[i] != 0) {
			if (max_diff < std::abs(diff_array[i])) {
				max_diff = std::abs(diff_array[i]);
			}
			signed_avg_diff += (double)(diff_array[i]);
			unsigned_avg_diff += abs((double)(diff_array[i]));
			counter++;
		}
	}
	
	signed_avg_diff /= (double)(array_length);
	unsigned_avg_diff /= (double)(array_length);

	double ratio = 100.0 * counter / (double)(array_length);

	std::cout << ratio << "% of computed roots don't exactly match." << std::endl;
	std::cout << "Greatest bit difference: " << max_diff << " bits" << std::endl;
	std::cout << "Average bit difference: " << signed_avg_diff << " bits" << std::endl;
	std::cout << "Average absolute bit difference: " << unsigned_avg_diff << " bits" << std::endl;

	std::printf("\n");

	delete[] rand_array;
	delete[] root_array_oroots;
	delete[] root_array_std;
	delete[] diff_array;

	system("PAUSE");
	return EXIT_SUCCESS;
}

int main() {
	int r = bench64();

	if (r != 0) {
		return r;
	}

	std::cout << std::endl;

	return bench32();
}
