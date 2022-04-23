#pragma once

#include"Example.h"

#include"Model/Model.h"

class CornellBoxExample : public Example
{
public:
	CornellBoxExample(Image& img, int maxDepth, int nSamples);

	void draw() override;
private:
};