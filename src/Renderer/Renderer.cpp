#include "Renderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>

#include"../Log.h"
#include"IntersectionTests.h"

std::vector<glm::vec3> tri =
{
	{0.f, 1, 1.f},
	{-1, -1.f, 1.f},
	{1, -1.f, 1.f},
};


Renderer::Renderer(const Image& image)
	:m_image(image)
{
	m_camera.position = {0, 0, 0};
	m_camera.rotation = {0, 0};

	m_image.data = new unsigned char[m_image.width * m_image.height * m_image.channels];
}

void Renderer::draw()
{
	for(int y = 0; y < m_image.height * m_image.channels; y += m_image.channels)
		for(int x = 0; x < m_image.width * m_image.channels; x += m_image.channels)
		{
			Ray r = getRayThroughPixel(x / m_image.channels, y / m_image.channels);
			if(Intersection::triangle_halfPlane(r, tri[0], tri[1], tri[2]))
			{
				m_image.data[index(x, y)] = 255;
				m_image.data[index(x, y) + 1] = 255;
				m_image.data[index(x, y) + 2] = 255;
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
	glm::vec3 pixelWorld;
	pixelWorld.x = 2.f * (((x + 0.5f) / m_image.width) - 0.5f) * m_image.aspectRatio * fovScale;
	pixelWorld.y = -2.f * (((y + 0.5f) / m_image.height) - 0.5f) * fovScale;
	pixelWorld.z = 1;

	return Ray(m_camera.position, glm::normalize(pixelWorld - m_camera.position));
}


