#include "lib/bitmap_image.hpp"
#include "ThreadUtils/Task/CFunctionalTask.h"
#include "ThreadUtils/Pool/CPool.h"
#include "ThreadUtils/BasicTaskExecutor/CBasicTaskExecutor.h"
#include <iostream>
#include <functional>
#include <string>
#include <memory>
#include <filesystem>
#include <ctime>

const int BLUR_RADIUS = 20;

struct SBlurBmpBlockSettings
{
	int startBlurPixelX = 0;
	int startBlurPixelY = 0;

	int endBlurPixelX = 0;
	int endBlurPixelY = 0;

	bitmap_image* inputImage;
	bitmap_image* outputImage = nullptr;

	int blurRadius = 0;
};

void BlurBmpBlock(SBlurBmpBlockSettings* settings)
{
	for (int height = settings->startBlurPixelY; height < settings->endBlurPixelY; ++height)
	{
		for (int width = settings->startBlurPixelX; width < settings->endBlurPixelX; ++width)
		{
			std::vector<rgb_t> pixels;

			for (int i = height - BLUR_RADIUS; i <= height + BLUR_RADIUS; ++i)
			{
				for (int j = width - BLUR_RADIUS; j <= width + BLUR_RADIUS; ++j)
				{
					int minHeight = std::min(int(settings->inputImage->height()) - 1, std::max(i, 0));

					int minWidth = std::min(int(settings->inputImage->width()), std::max(j, 0));

					rgb_t pixel = settings->inputImage->get_pixel(minWidth, minHeight);

					pixels.push_back(pixel);
				}
			}

			int totalRed = 0, totalGreen = 0, totalBlue = 0;

			for (const auto& pixel : pixels)
			{
				totalRed += pixel.red;
				totalGreen += pixel.green;
				totalBlue += pixel.blue;
			}

			rgb_t color;
			color.red = static_cast<uint8_t>(totalRed / pixels.size());
			color.green = static_cast<uint8_t>(totalGreen / pixels.size());
			color.blue = static_cast<uint8_t>(totalBlue / pixels.size());

			settings->outputImage->set_pixel(width, height, color);
		}
	}
}

std::vector<SBlurBmpBlockSettings*> CreateBlockSettingsForSingleImage(bitmap_image* inputImagePtr, bitmap_image* outputImagePtr, int blocksQuantity)
{
	std::vector<SBlurBmpBlockSettings*> result;

	unsigned int w = inputImagePtr->width();
	unsigned int h = outputImagePtr->height();

	for (int i = 0; i < blocksQuantity; ++i)
	{
		SBlurBmpBlockSettings* s = new SBlurBmpBlockSettings();
		s->blurRadius = BLUR_RADIUS;
		s->inputImage = inputImagePtr;
		s->outputImage = outputImagePtr;
		s->startBlurPixelY = 0;
		s->endBlurPixelY = h;

		const unsigned int wPerThread = w / blocksQuantity;

		s->startBlurPixelX = (wPerThread * i);

		if (i + 1 != blocksQuantity)
		{
			s->endBlurPixelX = wPerThread * (i + 1);
		}
		else
		{
			s->endBlurPixelX = w;
		}

		result.push_back(s);
	}

	return result;
}

int main(int argc, char* argv[])
{
	if (argc != 6) {
		std::cout << "Please provide input params: <mode=pool|basic> <blocksQuantity> <imageSourceDir> <imageOutDir> <threadsInPoolQuantity>" << std::endl;
		return 0;
	}

	std::string mode(argv[1]);
	int blocksQ = std::stoi(argv[2]);
	std::string inputImageDirectory(argv[3]);
	std::string outputImageDirectory(argv[4]);
	int threadsInPoolQ = std::stoi(argv[5]);

	if (!std::filesystem::is_directory(outputImageDirectory) || !std::filesystem::exists(outputImageDirectory))
	{
		std::filesystem::create_directory(outputImageDirectory);
	}

	std::clock_t start = std::clock();

	for (const auto& file : std::filesystem::directory_iterator(inputImageDirectory))
	{
		std::filesystem::path inputImagePath = file.path();

		auto inputBmp = bitmap_image(inputImagePath.string());
		auto outputBmp = new bitmap_image(inputBmp);

		auto blocks = CreateBlockSettingsForSingleImage(&inputBmp, outputBmp, blocksQ);

		std::vector<std::shared_ptr<ITask>> ptrs;

		for (auto block : blocks)
		{
			auto ptr = std::make_shared<CFunctionalTask>(std::bind(BlurBmpBlock, block));
			ptrs.push_back(ptr);
		}

		if (mode == "pool") {
			CPool pool(ptrs, threadsInPoolQ);
			pool.Execute();
		}
		else if (mode == "basic") {
			CBasicTaskExecutor taskExecutor(ptrs);
			taskExecutor.Execute();
		}
		else {
			throw std::exception("Mode is not defined");
		}

		outputBmp->save_image(outputImageDirectory + "/"+ file.path().filename().string());

		delete outputBmp;
	}

	std::clock_t end = std::clock();

	std::cout << "Runtime: " << end - start << std::endl;

	return 0;
}