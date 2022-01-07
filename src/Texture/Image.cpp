#include "Image.h"

Image::Image(unsigned width, unsigned height, unsigned channels)
	:width(width), height(height), channels(channels)
{
	calculateData();
}

Image::~Image()
{
	if(data != nullptr)
		delete[] data;
}

Image::Image(const Image& other)
{
	if(data != nullptr)
		delete[] data;

	width = other.width;
	height = other.height;
	channels = other.channels;
	step = other.step;
	aspectRatio = other.aspectRatio;
	data = other.data;
}

Image& Image::operator=(const Image& rhs)
{
	if(this != &rhs)
	{
		if(data != nullptr)
			delete[] data;

		width = rhs.width;
		height = rhs.height;
		channels = rhs.channels;
		step = rhs.step;
		aspectRatio = rhs.aspectRatio;
		data = rhs.data;
		return *this;
	}
}

Image::Image(Image&& other)
{
	width = other.width;
	height = other.height;
	channels = other.channels;
	step = other.step;
	aspectRatio = other.aspectRatio;
	data = other.data;

	other.width = 1;
	other.height = 1;
	other.channels = 1;
	other.step = 1;
	other.aspectRatio = 1;
	other.data = nullptr;
}

Image& Image::operator=(Image&& rhs)
{
	if(this != &rhs)
	{
		if(data != nullptr)
			delete[] data;

		width = rhs.width;
		height = rhs.height;
		channels = rhs.channels;
		step = rhs.step;
		aspectRatio = rhs.aspectRatio;
		data = rhs.data;

		rhs.width = 1;
		rhs.height = 1;
		rhs.channels = 1;
		rhs.step = 1;
		rhs.aspectRatio = 1;
		rhs.data = nullptr;

		return *this;
	}
}

void Image::calculateData()
{
	step = channels * width;
	aspectRatio = static_cast<float>(width) / height;
}
