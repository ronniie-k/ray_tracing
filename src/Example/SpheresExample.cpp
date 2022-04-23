#include "SpheresExample.h"

#include<vector>
#include<glm/geometric.hpp>
#include<glm/trigonometric.hpp>

#include"Log.h"
#include"Model/Material.h"

#include"Renderer/Models/Reflection.h"
#include"Random.h"


//different views

SpheresExample::SpheresExample(Image& img, int maxDepth, int nSamples)
	:Example(img, "res/models/CornellBoxSphere/CornellBoxOriginal.obj", {0, -1, -1.5}, maxDepth, nSamples)
{
	m_camera.position = {0, 0, 0};
	m_camera.target = {0, 0, -1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);

	Sphere s1({.5, .2, -2.}, 0.25f);
	Sphere s2({-.5, .2, -2.}, 0.25f);
	Sphere s3({0, .15, -1.5}, 0.25f);
	m_spheres.push_back(s1);
	m_spheres.push_back(s2);
}

void SpheresExample::draw()
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
		}
	//Log::info("abc");
	writeImage("spheres");
}