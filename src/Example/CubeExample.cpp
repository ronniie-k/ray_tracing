#include "CubeExample.h"

#include<vector>
#include<glm/geometric.hpp>

#include"Log.h"

std::vector<Material> mat;

Vertex v1 = {0, {0, 1, -3}, {0, 0, 1}, {0, 0}};
Vertex v2 = {0, {-1, 1, -3}, {0, 0, 1}, {0, 0}};
Vertex v3 = {0, {0, 0, -3}, {0, 0, 1}, {0, 0}};

CubeExample::CubeExample(Image& img)
	:Example(img, "res/models/cube/cube.obj", {0, 0, 3}, 1, 3), m_texture("res/textures/test.png"), m_triangle(v1, v2, v3, mat)
{
	m_camera.position = {0, 0, 0};
	m_camera.target = {0, 0, -1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

void CubeExample::draw()
{
	for(unsigned y = 0; y < m_image.height; y++)
		for(unsigned x = 0; x < m_image.width; x++)
		{
			glm::ivec2 pixel;

			Ray r = getRayThroughPixel(pixel.x, pixel.y);
			float u, v, t;
			if(m_triangle.intersection(r, t, u, v))
			{
				float depth = r(t).z;
				int depthBufferIndex = index(pixel.x, pixel.y);

				glm::vec3 color = {1, 1, 1}; //m_texture.getColor((1 - v - u) * m_triangle[0].uv + u * m_triangle[1].uv + v * m_triangle[2].uv);
				setPixelColor(x, y, color);
			}
		}
	writeImage("cube");
}
