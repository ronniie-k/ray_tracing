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
	BVH m_bvh;
};