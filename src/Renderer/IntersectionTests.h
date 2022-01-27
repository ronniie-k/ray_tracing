#pragma once

/// Holds all ray-primitive intersection tests
/// some primitives may have multiple variants e.g. triangle half plane and triangle barycentric

/*namespace Intersection
{
	namespace Triangle
	{
		bool halfPlane(Ray& r, float& t, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
		bool barycentric(Ray& r, float& t, const Vertex& v0, const Vertex& v1, const Vertex& v2, float& u, float& v);
		bool mollerTrumbore(Ray& r, float& t, const Triangle& tri, float& u, float& v);
	}
}*/