#pragma once

#include<string>
#include<glm/vec3.hpp>

//following https://en.wikipedia.org/wiki/Wavefront_.obj_file
struct Material
{
	std::string name;

	glm::vec3 ka = {1, 1, 1}; //ambient
	glm::vec3 kd = {1, 1, 1}; //diffuse
	glm::vec3 ks = {1, 1, 1}; //specular
	float ns = 0.f; //shininess
	float d = 1.f; //transparency, 1 = opaque
};