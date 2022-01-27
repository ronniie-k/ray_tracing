#include "Triangle.h"

#include<glm/geometric.hpp>

Triangle::Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Material>& materials)
	:m_vertices({v0, v1, v2}), m_materials(materials)
{
	m_materialID = v0.materialID;
}

Triangle::Triangle(const std::array<Vertex, 3>& vertices, std::vector<Material>& materials)
	:Triangle(vertices[0], vertices[1], vertices[2], materials)
{
}

const Material& Triangle::getMaterial()
{
	return m_materials[m_materialID];
}

const glm::vec3& Triangle::getNormal()
{
	return m_vertices[0].normal;
}


//code was taken from https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
//and modified for glm/c++

bool Triangle::intersection(Ray& r, float& t, float& u, float& v) const //moller trumbore
{
	glm::vec3 pvec, tvec, qvec;
	glm::vec3 e0, e1;
	float invDet;

	e0 = m_vertices[1].position - m_vertices[0].position;
	e1 = m_vertices[2].position - m_vertices[0].position;

	//scalar triple product
	pvec = glm::cross(r.direction, e1);
	float det = glm::dot(e0, pvec);
#ifdef CULL
	if(det < 0.00001f)
		return false;

	tvec = r.origin - v0.position;
	u = glm::dot(pvec, tvec);

	if(u < 0.f || u > det)
		return false;

	qvec = glm::cross(tvec, e0);
	v = glm::dot(r.direction, qvec);

	if(v < 0.f || u + v > det)
		return false;

	t = glm::dot(e1, qvec);

	invDet = 1.f / det;
	t *= invDet;
	u *= invDet;
	v *= invDet;
#else
	if(det > -0.00001f && det < 0.00001f)
		return 0;

	invDet = 1.f / det;

	tvec = r.origin - m_vertices[0].position;
	u = glm::dot(tvec, pvec) * invDet;
	if(u < 0.f || u > 1.f)
		return false;

	qvec = glm::cross(tvec, e0);
	v = glm::dot(r.direction, qvec) * invDet;
	if(v < 0.f || u + v > 1.f)
		return 0;

	t = glm::dot(e1, qvec) * invDet;
#endif // CULL
	return t > 0;
}

#if 0
bool Triangle::intersection(Ray& r, float& t) //half plane
{
	//ray-plane intersection
	glm::vec3 v0v1 = m_vertices[1].position - m_vertices[0].position;
	glm::vec3 v0v2 = m_vertices[2].position - m_vertices[0].position;
	glm::vec3 normal = glm::cross(v0v1, v0v2);

	float deno = glm::dot(normal, r.direction);
	float numerator = (glm::dot(normal, v0) + glm::dot(normal, r.origin));

	if(deno <= 0.0001) //divide by 0 = :(, this also means that r is (almost) perpendicular and will never hit
		return false;
	t = numerator / deno;

	if(t < 0) //plane is behind ray's origin and therefore camera
		return false;

	glm::vec3 hit = r(t);

	glm::vec3 e0 = m_vertices[1].position - m_vertices[0].position;
	glm::vec3 e1 = m_vertices[2].position - m_vertices[1].position;
	glm::vec3 e2 = m_vertices[0].position - m_vertices[2].position;

	glm::vec3 c0 = hit - m_vertices[0].position;
	glm::vec3 c1 = hit - m_vertices[1].position;
	glm::vec3 c2 = hit - m_vertices[2].position;

	//3 half plane tests
	return glm::dot(normal, glm::cross(e0, c0)) > 0
		&& glm::dot(normal, glm::cross(e1, c1)) > 0
		&& glm::dot(normal, glm::cross(e2, c2)) > 0;
}

bool Triangle::intersection(Ray& r, float& t, float& u, float& v) //barycentric
{
	//ray-plane intersection
	glm::vec3 e0 = m_vertices[1].position - m_vertices[0].position;
	glm::vec3 e1 = m_vertices[2].position - m_vertices[1].position;
	glm::vec3 e2 = m_vertices[0].position - m_vertices[2].position;
	glm::vec3 normal = m_vertices[0].normal;

	float deno = glm::dot(normal, r.direction);
	float numerator = (glm::dot(normal, v0.position) + glm::dot(normal, r.origin));

	if(std::abs(deno) <= 0.0001) //divide by 0 = :(, this also means that r is perpendicular (to normal so parallel to surface of tri) and will never hit
		return false;
	t = numerator / deno;

	if(t < 0) //plane is behind ray's origin and therefore camera
		return false;

	glm::vec3 hit = r(t);

	glm::vec3 c0 = hit - m_vertices[0].position;
	glm::vec3 c1 = hit - m_vertices[1].position;
	glm::vec3 c2 = hit - m_vertices[2].position;

	float area = glm::length(glm::cross(e0, e2));
	float alpha = glm::length(glm::cross(e1, c1)) / area;
	if(alpha < 0 || alpha > 1)
		return false;

	float beta = glm::length(glm::cross(e2, c2)) / area;
	if(beta < 0 || beta > 1)
		return false;

	float gamma = glm::length(glm::cross(e0, c0)) / area;
	if(gamma < 0 || gamma > 1)
		return false;

	if(alpha + beta + gamma > 1)
		return false;

	u = beta;
	v = gamma;
	return true;
	//idea behind this algorithm is to compute the area of the triangle and the triangles
	//which are created by making edges from v0, v1 and v2 to the point where r hits
	//the ratio of these areas gives the barycentric coordinates
}

#endif