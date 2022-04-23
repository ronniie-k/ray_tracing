#pragma once

#include<glm/vec3.hpp>
#include"Renderer/Ray.h"

//axis aligned bounding box
struct AABB
{
	AABB() = default;
	AABB(const glm::vec3& min, const glm::vec3& max);
	bool intersection(Ray& r, float& tStart, float& tEnd) const;

	glm::vec3 minExtent;
	glm::vec3 maxExtent;
};