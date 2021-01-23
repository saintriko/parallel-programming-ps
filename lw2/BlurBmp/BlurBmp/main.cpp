#include <iostream>
#include <algorithm>
#include "bitmap_image.hpp"
#include "SAsyncBlurBMPSettings.h"
#include "BlurBmp.h"
#include <ctime>

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cout << "Please provide args: <input image> <output image> <threads> <cores>";
		return 1;
	}

	try
	{
		const std::string inputImage = argv[1];
		const std::string outputImage = argv[2];
		const int threadCount = std::stoi(argv[3]);
		const int coreCount = std::stoi(argv[4]);

		const SAsyncBlurBMPSettings settings(inputImage, outputImage, size_t(coreCount), size_t(threadCount));

		std::clock_t start = std::clock();

		BlurBmp(settings);

		std::clock_t end = std::clock();

		std::cout << "Runtime: " << std::difftime(end, start) << " ms" << std::endl;
	}
	catch (const std::invalid_argument&)
	{
		return 1;
	}

	return 0;
}