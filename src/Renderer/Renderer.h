#pragma once

#include<vector>

#include"Camera.h"
#include"Ray.h"
#include"Texture/Image.h"

#include"Model/Model.h"
#include"Texture/Texture.h"

class Renderer
{
public:
	Renderer(const Image& image);

	void draw();
private:
	int index(int x, int y);
	Ray getRayThroughPixel(int x, int y);
	void setPixelColor(int x, int y, const glm::vec3& color);
private:
	Camera m_camera;
	Image m_image;
	float* m_depthBuffer;

	Model m_cube;
	Texture m_texture;
};