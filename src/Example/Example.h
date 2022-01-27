#pragma once

//base class used for all rendering examples
//e.g. rendering a cube, cornell box,
//cornell box using blinn phong, etc...

#include"Texture/Image.h"
#include"Renderer/Ray.h"
#include"Renderer/Camera.h"

class Example
{
public:
	Example(Image& img);

	virtual void draw() = 0;
protected:
	int index(int x, int y);
	Ray getRayThroughPixel(int x, int y);
	void setPixelColor(int x, int y, const glm::vec3& color);

	//depth test, check if d is closer to the camera than what is currently stored
	//returns true if d is closer
	bool depthTest(float d, int index);
protected:
	Camera m_camera;
	Image& m_image;
	float* m_depthBuffer;
};