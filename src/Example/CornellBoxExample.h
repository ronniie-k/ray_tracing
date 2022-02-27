#pragma once

#include"Example.h"

#include"Model/Model.h"

class CornellBoxExample : public Example
{
public:
	CornellBoxExample(Image& img);

	void draw() override;
	glm::vec3 tracePath(const glm::vec2& pixel, Ray& r, int depth);
private:
	bool inShadow(Ray& r);
private:
	Model m_cornellBox;
};