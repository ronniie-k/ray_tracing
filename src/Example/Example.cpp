#include "Example.h"

#include<glm/vec3.hpp>
#include<glm/geometric.hpp>

Example::Example(Image& img)
	:m_image(img)
{
	//initialize everything
	m_depthBuffer = new float[m_image.width * m_image.height];
	constexpr float inf = -std::numeric_limits<float>::infinity();

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

int Example::index(int x, int y)
{
	return y * m_image.width + x;
}

Ray Example::getRayThroughPixel(int x, int y)
{
	float fovScale = 1.428184f; //tan55
	glm::vec3 pixelToWorld(0.f);
	pixelToWorld += 2.f * (((x + 0.5f) / m_image.width) - 0.5f) * m_image.aspectRatio * fovScale * m_camera.right;
	pixelToWorld += -2.f * (((y + 0.5f) / m_image.height) - 0.5f) * fovScale * m_camera.up;
	pixelToWorld += -m_camera.position + m_camera.lookAt;
	return Ray(m_camera.position, glm::normalize(pixelToWorld));
}

void Example::setPixelColor(int x, int y, const glm::vec3& color)
{
	int imageIndex = index(x, y);
	m_image.data[imageIndex + 0] = static_cast<unsigned char>(color.r);
	m_image.data[imageIndex + 1] = static_cast<unsigned char>(color.g);
	m_image.data[imageIndex + 2] = static_cast<unsigned char>(color.b);
}

bool Example::depthTest(float d, int index)
{
	if(d > m_depthBuffer[index])
	{
		m_depthBuffer[index] = d;
		return true;
	}
	return false;
}
