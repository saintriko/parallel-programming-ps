#include <iostream>
#include "SAsyncBlurBMPSettings.h"
#include "BlurBmp.h"
#include <ctime>

int HandleInvalidInput()
{
	std::cout << "Please provide args: <input image> <output image> <threads> <cores> <thread priorities: -1 | 0 | 1>";
	return 1;
}

int main(int argc, char* argv[])
{
	if ((argc == 2 && strcmp(argv[1], "/") == 0) || (argc < 5))
	{
		return HandleInvalidInput();
	}

	try
	{
		const std::string inputImage = argv[1];
		const std::string outputImage = argv[2];
		const int threadCount = std::stoi(argv[3]);
		const int coreCount = std::stoi(argv[4]);

		std::vector<int> priorities;

		if (argc < 5 + threadCount)
		{
			return HandleInvalidInput();
		}

		for (int i = 5; i < 5 + threadCount; ++i)
		{
			priorities.push_back(std::stoi(argv[i]));
		}

		const SAsyncBlurBMPSettings settings(inputImage, outputImage, coreCount, threadCount, priorities);

		BlurBmp(settings);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		
		return 1;
	}

	return 0;
}
