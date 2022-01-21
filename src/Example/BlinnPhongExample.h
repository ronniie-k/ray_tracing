#pragma once

#include"Example.h"

#include"Model/Model.h"
#include"Texture/Texture.h"

class BlinnPhongExample : public Example
{
public:
	BlinnPhongExample(Image& img);

	void draw() override;
private:
	Model m_cube;
	Texture m_texture;
};