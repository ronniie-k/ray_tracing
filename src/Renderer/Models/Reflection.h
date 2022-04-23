#pragma once

#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
#include<glm/geometric.hpp>

#include"Model/Material.h"

namespace BRDF
{
	glm::vec3 blinnPhong(const glm::vec3& wi, const glm::vec3& wo, const glm::vec3& normal, const Material& mat);
}