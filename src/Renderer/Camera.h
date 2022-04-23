#pragma once

#include<glm/vec3.hpp>
#include<glm/vec2.hpp>

struct Camera
{
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 lookAt;
	glm::vec3 up;
	glm::vec3 right;
};