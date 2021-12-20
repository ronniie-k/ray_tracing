#pragma once

struct Image
{
	Image() = default;
	Image(unsigned width, unsigned height, unsigned channels)
		:width(width), height(height), channels(channels)
	{
		calculateData();
	}

	void calculateData()
	{
		step = channels * width;
		aspectRatio = static_cast<float>(width) / height;
	}

	unsigned width = 1;
	unsigned height = 1;
	unsigned channels = 1;
	unsigned step = 1;
	float aspectRatio = 1;
	unsigned char* data = nullptr;
};
