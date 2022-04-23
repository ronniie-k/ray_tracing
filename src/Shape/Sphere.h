#pragma once

#include"Renderer/Ray.h"
#include"Model/Material.h"
#include<glm/vec3.hpp>

class Sphere
{
public:
	Sphere(const glm::vec3& center, float radius);
	//const Material& getMaterial() const;
	const glm::vec3& getNormal(const glm::vec3& point) const;
	const glm::vec3& getCenter() const;
	float getRadius() const;

	bool intersection(Ray& r, float& t);
private:
	glm::vec3 m_center = glm::vec3(0.f);
	float m_radius = 1.f;
};