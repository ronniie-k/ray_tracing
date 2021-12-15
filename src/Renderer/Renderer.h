#pragma once

#include<vector>

#include"Camera.h"
#include"Ray.h"

struct Image
{
	unsigned width = 1;
	unsigned height = 1;
	unsigned channels = 1;
	unsigned step = 1;
	float aspectRatio = 1;
	unsigned char* data = nullptr;
};

class Renderer
{
public:
	Renderer(const Image& image);

	void draw();
private:
	int index(int x, int y);
	Ray getRayThroughPixel(int x, int y);
private:
	Camera m_camera;
	Image m_image;
};