#pragma once

#include<string>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>

#include"Image.h"

class Texture
{
public:
	Texture(const std::string& file);
	~Texture();

	glm::vec3 getColor(const glm::vec2& uv) const;
private:
	Image m_data;
};