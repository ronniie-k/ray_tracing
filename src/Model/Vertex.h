#pragma once

#include<glm/vec2.hpp>
#include<glm/vec3.hpp>
#include<string>

struct Vertex
{
	int materialID = 0;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};