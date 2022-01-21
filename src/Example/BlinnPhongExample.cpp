#include "BlinnPhongExample.h"

#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>

#include"Log.h"
#include"Renderer/IntersectionTests.h"

float BlinnPhong(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const glm::vec3& lightPos, const glm::vec3& normal);

BlinnPhongExample::BlinnPhongExample(Image& img)
	:Example(img), m_cube("res/models/cube/cube.obj", {0, 0, 3}), m_texture("res/textures/test.png")
{
	m_camera.position = {3, 0, 0};
	m_camera.target = {2, 0, 1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

void BlinnPhongExample::draw()
{
	for(unsigned y = 0; y < m_image.height * m_image.channels; y += m_image.channels)
		for(unsigned x = 0; x < m_image.width * m_image.channels; x += m_image.channels)
		{
			glm::ivec2 pixel;
			pixel.x = x / m_image.channels;
			pixel.y = y / m_image.channels;

			for(unsigned i = 0; i < m_cube.size(); i += 3)
			{
				Ray r = getRayThroughPixel(pixel.x, pixel.y);
				float u, v, t;
				if(Intersection::Triangle::mollerTrumbore(r, t, m_cube[i], m_cube[i + 1], m_cube[i + 2], u, v))
				{
					float depth = r(t).z;
					int depthBufferIndex = index(pixel.x, pixel.y);

					if(depth < m_depthBuffer[depthBufferIndex])
					{
						m_depthBuffer[depthBufferIndex] = depth;
						glm::vec3 color = m_texture.getColor((1 - v - u) * m_cube[i].uv + u * m_cube[i + 1].uv + v * m_cube[i + 2].uv);
						float light = BlinnPhong(m_camera.position, r(t), {4, 1, 5}, m_cube[i].normal);
						setPixelColor(x, y, color * light);
					}
				}
				else
				{
					//
				}
			}
		}
	stbi_write_png("output/BlinnPhongExample.png", m_image.width, m_image.height, m_image.channels, m_image.data, m_image.step);
}

//temp for now
float BlinnPhong(const glm::vec3& cameraPos, const glm::vec3& pixelPos, const glm::vec3& lightPos, const glm::vec3& normal)
{
	static float kd = 0.8f; //diffuse reflectance
	static float ks = 1.2f; //specular reflectance
	static float ka = 0.1f; //ambient reflectance
	static float shininess = 32;

	glm::vec3 lightDir = lightPos - pixelPos;

	//diffuse
	glm::vec3 N = glm::normalize(normal);
	glm::vec3 L = glm::normalize(lightDir);
	float diffuse = kd * glm::max(glm::dot(N, L), 0.f);

	//specular
	glm::vec3 reflection = 2.f * glm::dot(L, N) * (N - L);
	glm::vec3 viewDir = glm::normalize(cameraPos - pixelPos);
	float specular = glm::pow(glm::max(glm::dot(viewDir, reflection), 0.f), shininess);

	float ambient = ka;

	return diffuse + specular + ambient;
}