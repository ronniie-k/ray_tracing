#pragma once

#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
#include<glm/geometric.hpp>

#include"Model/Material.h"

//reference
//https://www.pbr-book.org/3ed-2018/Reflection_Models/Basic_Interface#BxDF

/*enum BxDFType
{
	BSDFReflection	 = 1 << 0,
	BSDFTransmission = 1 << 1,
	BSDFDiffuse		 = 1 << 2,
	BSDFGlossy		 = 1 << 3,
	BSDFSpecular     = 1 << 4,
	BSDFALL = BSDFReflection | BSDFTransmission | BSDFDiffuse | BSDFGlossy | BSDFSpecular
};

class BxDF
{
public:
	BxDF(BxDFType type)
		:m_type(type) {}

	bool matchType(BxDFType type) { return (m_type & type) == m_type; }
	//wi = incident light, wo = outgoing light
	virtual Spectrum f(const glm::vec3& wo, const glm::vec3& wi) const = 0;
	virtual Spectrum sample_f(const glm::vec3& wo, const glm::vec3& wi, const glm::vec2& sample, float* pdf, BxDFType* type = nullptr) const;
	//hemispherical directional reflectance
	virtual Spectrum rho(const glm::vec3& w0, int nSamples, const glm::vec2* samples) const;
	virtual Spectrum rho(int nSamples, const glm::vec2* samples) const;
private:
	const BxDFType m_type;
};*/


namespace BRDF
{
	glm::vec3 blinnPhong(const glm::vec3& wi, const glm::vec3& wo, const glm::vec3& normal, const Material& mat);
}