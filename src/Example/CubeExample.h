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
	Model m_cube;
	Texture m_texture;
};