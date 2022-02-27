#include "CornellBoxExample.h"

#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>
#include<glm/trigonometric.hpp>

#include"Log.h"
#include"Renderer/IntersectionTests.h"
#include"Model/Material.h"

#include"Renderer/Models/Reflection.h"
#include"Random.h"

CornellBoxExample::CornellBoxExample(Image& img)
	:Example(img), m_cornellBox("res/models/CornellBoxOriginal/CornellBoxOriginal.obj", {0, -1, -1.5})
{
	m_camera.position = {0, 0, 0};
	m_camera.target = {0, 0, -1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

void CornellBoxExample::draw()
{
	int nSamples = 10; //temp
	for(unsigned y = 0; y < m_image.height; y++)
	for(unsigned x = 0; x < m_image.width; x++)
	{
		glm::vec3 color(0);
		glm::vec2 pixel;
		pixel.x = x;
		pixel.y = y;

		for(int n = 0; n < nSamples; n++)
		{
			float xOff = Random::getFloatInRange(-0.5f, 0.5f);
			float yOff = Random::getFloatInRange(-0.5f, 0.5f);
			Ray r = getRayThroughPixel(pixel.x + xOff, pixel.y + yOff);
			color += tracePath(r, 0);
		}

		color /= nSamples;
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
	Log::info("abc");
	stbi_write_png("output/cornellBox.png", m_image.width, m_image.height, m_image.channels, m_image.data, m_image.step);
}


//references
//https://www.youtube.com/watch?v=odXCvJTNn6s&list=PLQ3UicqQtfNuBjzJ-KEWmG1yjiRMXYKhh&index=16
//https://en.wikipedia.org/wiki/Path_tracing

//problem:
//not considering closest tri, so stuff gets messed up
//to fix, go through all tris find closest
//then do the path tracing stuff

glm::vec3 CornellBoxExample::tracePath(Ray& r, int depth)
{
	static int maxDepth = 5;
	static float p = 1 / 6.28318530718f;

	if(depth >= maxDepth)
		return glm::vec3(0);

	glm::vec3 color(0);
	Triangle* closest = nullptr;
	float tMin = std::numeric_limits<float>::infinity();

	//get closest intersection
	for(Triangle& tri : m_cornellBox)
	{
		float u, v, t;

		if(tri.intersection(r, t, u, v))
		{
			if(t < tMin)
			{
				tMin = t;
				closest = &tri;
			}
		}
	}
	if(closest == nullptr || tMin < 0 || tMin == std::numeric_limits<float>::infinity())
		return glm::vec3(0);

	glm::vec3 lightPos = {0, 0.85, -1.55};
	glm::vec3 intersectionPoint = r(tMin);

	//shadow ray
	Material mat = closest->getMaterial();
	glm::vec3 shadowDir = glm::normalize(lightPos - intersectionPoint);
	Ray shadow(intersectionPoint + (0.1f * shadowDir), shadowDir);

	//if(inShadow(r))
	//	return glm::vec3(0);

	//generate secondary ray
	Ray secondary;

	//reference
	//https://gist.github.com/andrewbolster/10274979
	//generate random unit vector
	float phi = Random::getFloatInRange(0.f, 6.28318530718f); //phi = [0, 2pi]
	float costheta = Random::getFloatInRange(-1, 1);
	float theta = glm::acos(costheta);

	glm::vec3 dir;
	dir.x = glm::sin(theta) * glm::cos(phi);
	dir.y = glm::sin(theta) * glm::sin(phi);
	dir.z = costheta;

	secondary.origin = intersectionPoint + 0.1f * dir;
	secondary.direction = dir;

	float dp = glm::dot(dir, closest->getNormal());

	//brdf
	float distance = glm::distance(lightPos, intersectionPoint);
	float attenuation = 1.f / (distance * distance);

	glm::vec3 wi = lightPos - intersectionPoint;
	glm::vec3 wo = m_camera.position - intersectionPoint;

	glm::vec3 emittance = {0, 0, 0}; //temp
	glm::vec3 light = glm::vec3(255) * glm::min(BRDF::blinnPhong(wi, wo, closest->getNormal(), mat) * attenuation, {1, 1, 1});

	glm::vec3 li = tracePath(secondary, depth + 1);
	glm::vec3 lo = (li + light);// *dp / p;
	return glm::min(glm::vec3(255), lo);
	return light;
}

bool CornellBoxExample::inShadow(Ray& r)
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