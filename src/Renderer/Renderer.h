#pragma once

#include<vector>

#include"Camera.h"
#include"Ray.h"
#include"../Texture/Image.h"

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