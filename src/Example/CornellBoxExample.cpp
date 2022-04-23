#include "CornellBoxExample.h"

#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>
#include<glm/trigonometric.hpp>

#include"Log.h"
#include"Model/Material.h"

#include"Renderer/Models/Reflection.h"
#include"Random.h"

CornellBoxExample::CornellBoxExample(Image& img, int maxDepth, int nSamples)
	:Example(img, "res/models/CornellBoxOriginal/CornellBoxOriginal.obj", {0, -1, -1.5}, maxDepth, nSamples)
{
	m_camera.position = {0, 0, 0};
	m_camera.target = {0, 0, -1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

void CornellBoxExample::draw()
{
	float invSamples = 1.f / m_nSamples;
	for(unsigned y = 0; y < m_image.height; y++)
	for(unsigned x = 0; x < m_image.width; x++)
	{
		glm::vec3 color(0);
		glm::vec2 pixel;
		pixel.x = x;
		pixel.y = y;

		for(int n = 0; n < m_nSamples; n++)
		{
			float xOff = Random::getFloatInRange(-0.5f, 0.5f);
			float yOff = Random::getFloatInRange(-0.5f, 0.5f);
			Ray r = getRayThroughPixel(pixel.x + xOff, pixel.y + yOff);
			color += tracePath(r, 0);
		}

		color *= invSamples;
		setPixelColor(x, y, color);

		/*for(Triangle& tri : m_cornellBox)
		{
			Ray r = getRayThroughPixel(pixel.x, pixel.y);
			float u, v, t;

			if(tri.intersection(r, t, u, v))
			{
				glm::vec3 intersectionPos = r(t);
				int depthBufferIndex = index(pixel.x, pixel.y);

				if(depthTest(intersectionPos.z, depthBufferIndex))
				{
					//declare stuff here
					glm::vec3 color = {255, 255, 255};
					glm::vec3 lightPos = {0, 0.85, -1.55};

					//shadow ray
					glm::vec3 dir = glm::normalize(lightPos - intersectionPos);
					Ray shadow(intersectionPos + (0.1f * dir), dir);

					//brdf declarations
					float distance = glm::distance(lightPos, intersectionPos);
					float attenuation = 1.f / (distance * distance);

					glm::vec3 wi = lightPos - intersectionPos;
					glm::vec3 wo = m_camera.position - intersectionPos;

					Material mat = tri.getMaterial();

					//brdf()
					if(mat.name == "light" || !inShadow(shadow))
					{
						glm::vec3 light = ((mat.ka * 0.1f) + BRDF::blinnPhong(wi, wo, tri.getNormal(), mat)) * attenuation;
						color *= glm::min(light, {1, 1, 1});
					}
					else
						color *= (mat.ka * 0.1f);
						
					setPixelColor(x, y, color);
				}
			}
		}*/
	}
	writeImage("cornellBox");
}