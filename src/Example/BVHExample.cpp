#include "BVHExample.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>

#include"Log.h"
#include"Model/Material.h"

#include"Accelerations/BVH.h"
#include"Shape/IntersectionInfo.h"

inline glm::vec3 BlinnPhong(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const glm::vec3& lightPos, const glm::vec3& normal, const Material& mat);

BVHExample::BVHExample(Image& img)
	:Example(img), m_cornellBox("res/models/CornellBoxOriginal/CornellBoxOriginal.obj", {0, -1, -1.5}), m_bvh(m_cornellBox.getTriangles(), SplitMethod::SortedMedian)
{
	m_camera.position = {0, 0, 0};
	m_camera.target = {0, 0, -1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

void BVHExample::draw()
{
	for(unsigned y = 0; y < m_image.height * m_image.channels; y += m_image.channels)
		for(unsigned x = 0; x < m_image.width * m_image.channels; x += m_image.channels)
		{
			glm::ivec2 pixel;
			pixel.x = x / m_image.channels;
			pixel.y = y / m_image.channels;

			Ray r = getRayThroughPixel(pixel.x, pixel.y);
			IntersectionInfo info;

			if(m_bvh.intersect(r, info))
			{
				glm::vec3 intersectionPos = r(info.t);
				int depthBufferIndex = index(pixel.x, pixel.y);

				if(depthTest(intersectionPos.z, depthBufferIndex))
				{
					glm::vec3 color = {255, 255, 255};
					glm::vec3 lightPos = {0, 0.85, -1.55};

					glm::vec3 dir = glm::normalize(lightPos - intersectionPos);
					Ray shadow(intersectionPos + (0.01f * dir), dir);

					Material mat = info.tri->getMaterial();
					glm::vec3 light = ((mat.ka * 0.2f) + BlinnPhong(m_camera.position, intersectionPos, lightPos, info.tri->getNormal(), mat));

					if(mat.name == "light" || !inShadow(shadow))
					{
						//glm::vec3 light = ((mat.ka * 0.2f) + BlinnPhong(m_camera.position, intersectionPos, lightPos, tri.getNormal(), mat));
						color *= glm::min(light, {1, 1, 1});
					}
					else
						color *= (mat.ka * 0.2f);

					setPixelColor(x, y, color);
				}
			}
		}
	Log::info("abc");
	stbi_write_png("output/bvh.png", m_image.width, m_image.height, m_image.channels, m_image.data, m_image.step);
}

bool BVHExample::inShadow(Ray& r)
{
	for(Triangle& tri : m_cornellBox)
	{
		if(tri.getMaterial().name == "light")
			continue;
		float t, u, v;
		//the tri material checks here are temp until i figure out why the entire scene becomes enveloped in a shadow when i dont do this
		if(tri.intersection(r, t, u, v) && (tri.getMaterial().name == "tallBox" || tri.getMaterial().name == "shortBox"))
		{
			return true;
		}
	}
	return false;
}

glm::vec3 BlinnPhong(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const glm::vec3& lightPos, const glm::vec3& normal, const Material& mat)
{
	static float kd = 0.8f; //diffuse reflectance
	static float ks = 1.2f; //specular reflectance
	static float ka = 0.1f; //ambient reflectance
	static float shininess = 32;

	glm::vec3 lightDir = lightPos - pixelPos;

	//diffuse
	glm::vec3 N = glm::normalize(normal);
	glm::vec3 L = glm::normalize(lightDir);
	glm::vec3 diffuse = mat.kd * glm::max(glm::dot(N, L), 0.f);

	//specular
	glm::vec3 reflection = 2.f * glm::dot(L, N) * (N - L);
	glm::vec3 viewDir = glm::normalize(cameraPos - pixelPos);
	glm::vec3 specular = mat.ks + glm::pow(glm::max(glm::dot(viewDir, reflection), 0.f), mat.ns);

	return diffuse + specular;
}