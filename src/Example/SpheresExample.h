#pragma once

#include"Example.h"

#include"Model/Model.h"
#include"Shape/Sphere.h"

class SpheresExample : public Example
{
public:
	SpheresExample(Image& img, int maxDepth, int nSamples);

	void draw() override;
private:
};