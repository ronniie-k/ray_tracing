#pragma once

#include"Example.h"

#include"Model/Model.h"
#include"Accelerations/BVH.h"

class BVHExample : public Example
{
public:
	BVHExample(Image& img);

	void draw() override;
private:
	bool inShadow(Ray& r);
private:
	Model m_cornellBox;
	Model m_cube;
	BVH m_bvh;
	Texture m_texture;
};