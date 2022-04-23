#pragma once

struct Image
{
	Image() = default;
	Image(unsigned width, unsigned height, unsigned channels);
	~Image();

	Image(const Image& other);
	Image& operator=(const Image& rhs);

	Image(Image&& other);
	Image& operator=(Image&& rhs);

	void calculateData();

	unsigned width = 1;
	unsigned height = 1;
	unsigned channels = 1;
	unsigned step = 1;
	float aspectRatio = 1;
	unsigned char* data = nullptr;
};
