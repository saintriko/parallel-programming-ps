#pragma once
#include <string>
#include <ostream>
#include <vector>

struct SAsyncBlurBMPSettings
{
	SAsyncBlurBMPSettings(std::string inputImageName, std::string outputImageName, int coresCount, int threadsCount, std::vector<int> priority)
		: inputImageName(std::move(inputImageName))
		, outputImageName(std::move(outputImageName))
		, coresCount(coresCount)
		, threadsCount(threadsCount)
		, threadPriorities(std::move(priority))
	{	
		if (int(threadPriorities.size()) != threadsCount)
		{
			throw std::exception("Thread priorities size should be equal to threads count");
		}
	}

	std::string inputImageName;
	std::string outputImageName;

	int coresCount;
	int threadsCount;

	
	std::vector<int> threadPriorities;
};
