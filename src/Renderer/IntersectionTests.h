#pragma once

#include<glm/vec3.hpp>

#include"Ray.h"

/// Holds all ray-primitive intersection tests
/// some primitives may have multiple variants e.g. triangle half plane and triangle barycentric

namespace Intersection
{
	bool triangle_halfPlane(Ray& r, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
	bool triangle_barycentric(Ray& r, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
}