#pragma once
#include<glm/vec3.hpp>

struct Ray
{
	Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		:origin(origin), direction(direction) {}

	glm::vec3 origin;
	glm::vec3 direction;

	glm::vec3 operator()(float t) { return origin + t * direction; }
};