#include "Renderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>

#include"Log.h"
#include"IntersectionTests.h"

Renderer::Renderer(const Image& image)
	:m_image(image), m_cube("res/models/cube/cube.obj"), m_texture("res/textures/test.png")
{
	//fill data with empty values
	m_depthBuffer = new float[m_image.width * m_image.height];
	constexpr float inf = std::numeric_limits<float>::infinity();

	for(unsigned y = 0; y < m_image.height; y++)
		for(unsigned x = 0; x < m_image.width; x++)
			m_depthBuffer[index(x, y)] = inf;

	for(unsigned y = 0; y < m_image.height * m_image.channels; y++)
		for(unsigned x = 0; x < m_image.width * m_image.channels; x++)
			m_image.data[index(x, y)] = 0;


	m_camera.position = {0, 0, 0};
	m_camera.target = {0, 0, 1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

void Renderer::draw()
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
			if(Intersection::Triangle::barycentric(r, t, m_cube[i], m_cube[i + 1], m_cube[i + 2], u, v))
			{
				float depth = r(t).z;
				int depthBufferIndex = index(pixel.x, pixel.y);

				if(depth < m_depthBuffer[depthBufferIndex])
				{
					m_depthBuffer[depthBufferIndex] = depth;
					glm::vec3 color = m_texture.getColor((1 - v - u) * m_cube[i].uv + u * m_cube[i + 1].uv + v * m_cube[i + 2].uv);
					setPixelColor(x, y, color);
				}
			}
			else
			{
				//
			}
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
	float fovScale = 1.428184f; //tan55
	glm::vec3 pixelToWorld(0.f);
	pixelToWorld += 2.f * (((x + 0.5f) / m_image.width) - 0.5f) * m_image.aspectRatio * fovScale * m_camera.right;
	pixelToWorld += -2.f * (((y + 0.5f) / m_image.height) - 0.5f) * fovScale * m_camera.up;
	pixelToWorld += -m_camera.position + m_camera.lookAt;
	return Ray(m_camera.position, glm::normalize(pixelToWorld));
}

void Renderer::setPixelColor(int x, int y, const glm::vec3& color)
{
	int imageIndex = index(x, y);
	m_image.data[imageIndex + 0] = static_cast<unsigned char>(color.r);
	m_image.data[imageIndex + 1] = static_cast<unsigned char>(color.g);
	m_image.data[imageIndex + 2] = static_cast<unsigned char>(color.b);
}


