#pragma once

#include"Example.h"

#include"Model/Model.h"
#include"Texture/Texture.h"

class CubeExample : public Example
{
public:
	CubeExample(Image& img);

	void draw() override;
private:
	Texture m_texture;
	Triangle m_triangle;
};