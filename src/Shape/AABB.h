#pragma once

#include<glm/vec3.hpp>
#include"Renderer/Ray.h"

//axis aligned bounding box
struct AABB
{
	AABB() = default;
	bool intersection(Ray& r, float& tStart, float& tEnd);

	glm::vec3 minExtent;
	glm::vec3 maxExtent;
};