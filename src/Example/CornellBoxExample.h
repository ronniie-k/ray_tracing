#pragma once

#include"Example.h"

#include"Model/Model.h"

class CornellBoxExample : public Example
{
public:
	CornellBoxExample(Image& img);

	void draw() override;
private:
	Model m_cornellBox;
};