#include "Renderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include"../Log.h"
#include"IntersectionTests.h"
#include"../Texture/Texture.h"

std::vector<glm::vec3> tri =
{
	{1, 1, 1},
	{-1, -1, 1},
	{1, -1, 1},
};

std::vector<glm::vec3> colors =
{
	{1, 0, 0},
	{0, 1, 0},
	{1, 0, 1},
};

std::vector<glm::vec2> texcoords =
{
	{1, 1},
	{0, 0},
	{1, 0},
};

Renderer::Renderer(const Image& image)
	:m_image(image)
{
	glm::vec3 up = {0, 1, 0};
	m_camera.position = {0, 0.5, 0};
	m_camera.target = {0, 0, 1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross(up, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

void Renderer::draw()
{
	Texture t("res/textures/test.png");
	for(int y = 0; y < m_image.height * m_image.channels; y += m_image.channels)
		for(int x = 0; x < m_image.width * m_image.channels; x += m_image.channels)
		{
			Ray r = getRayThroughPixel(x / m_image.channels, y / m_image.channels);
			float u, v;
			if(Intersection::triangle_barycentric(r, tri[0], tri[1], tri[2], u, v))
			{
				//Log::info("u: {}, v: {}, w: {}", u, v, 1 - u - v);
				glm::vec3 color = glm::vec3(0) + u * colors[0] + v * colors[1] + (1 - u - v) * colors[2];
				color = t.getColor(u * texcoords[0] + v * texcoords[1] + (1 - u - v) * texcoords[2]);
				//Log::info("u: {}, v: {}, w: {}", color.r, color.g, color.b);

				/*m_image.data[index(x, y)] = 255 * color.r;
				m_image.data[index(x, y) + 1] = 255 * color.g;
				m_image.data[index(x, y) + 2] = 255 * color.b;*/

				m_image.data[index(x, y)] = color.r;
				m_image.data[index(x, y) + 1] = color.g;
				m_image.data[index(x, y) + 2] = color.b;
			}
			else
			{
				m_image.data[index(x, y)] = 0;
				m_image.data[index(x, y) + 1] = 0;
				m_image.data[index(x, y) + 2] = 0;
			}
		}
	stbi_write_png("output/test.png", m_image.width, m_image.height, m_image.channels, m_image.data, m_image.step);
}

int Renderer::index(int x, int y)
{
	return y * m_image.width + x;
}

Ray Renderer::getRayThroughPixel(int x, int y)
{
	float fovScale = 1.428184; //tan55
	glm::vec3 pixelWorld(0.f);
	pixelWorld += 2.f * (((x + 0.5f) / m_image.width) - 0.5f) * m_image.aspectRatio * fovScale * m_camera.right;
	pixelWorld += -2.f * (((y + 0.5f) / m_image.height) - 0.5f) * fovScale * m_camera.up;
	pixelWorld += m_camera.position + m_camera.lookAt;

	return Ray(m_camera.position, glm::normalize(pixelWorld - m_camera.position));
}


