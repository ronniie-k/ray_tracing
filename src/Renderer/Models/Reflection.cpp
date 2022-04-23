#include "Reflection.h"

#include<chrono>
#include"Timer.h"

glm::vec3 BRDF::blinnPhong(const glm::vec3& wi, const glm::vec3& wo, const glm::vec3& normal, const Material& mat)
{
	{
		//auto begin = std::chrono::high_resolution_clock::now();

		//diffuse
		glm::vec3 N = glm::normalize(normal);
		glm::vec3 L = glm::normalize(wi);
		glm::vec3 diffuse = mat.kd * glm::max(glm::dot(N, L), 0.f);

		//specular
		glm::vec3 reflection = 2.f * glm::dot(L, N) * (N - L);
		glm::vec3 viewDir = glm::normalize(wo);
		glm::vec3 specular = mat.ks + glm::pow(glm::max(glm::dot(viewDir, reflection), 0.f), mat.ns);

		//auto end = std::chrono::high_resolution_clock::now();
		//Timer::add("blinnPhong()", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());

		return (diffuse + specular);// *0.75f;
	}
}
