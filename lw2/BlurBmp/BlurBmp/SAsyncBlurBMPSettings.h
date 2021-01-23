#pragma once
#include <string>
#include <ostream>

struct SAsyncBlurBMPSettings {
	SAsyncBlurBMPSettings(std::string inputImageName, std::string outputImageName, int coresCount, int threadsCount)
	: inputImageName(std::move(inputImageName))
	, outputImageName(std::move(outputImageName))
	, coresCount(coresCount)
	, threadsCount(threadsCount)
	{
	}

	std::string inputImageName;
	std::string outputImageName;

	int coresCount;
	int threadsCount;
};