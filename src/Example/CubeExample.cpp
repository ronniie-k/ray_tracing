#include "CubeExample.h"


#include<stb_image_write.h>
#include<vector>
#include<glm/geometric.hpp>

#include"Log.h"
#include"Renderer/IntersectionTests.h"

CubeExample::CubeExample(Image& img)
	:Example(img), m_cube("res/models/cube/cube.obj", {0, 0, 3}), m_texture("res/textures/test.png")
{
}

void CubeExample::draw()
{
	for(unsigned y = 0; y < m_image.height * m_image.channels; y += m_image.channels)
		for(unsigned x = 0; x < m_image.width * m_image.channels; x += m_image.channels)
		{
			glm::ivec2 pixel;
			pixel.x = x / m_image.channels;
			pixel.y = y / m_image.channels;

			for(Triangle& tri : m_cube)
			{
				Ray r = getRayThroughPixel(pixel.x, pixel.y);
				float u, v, t;
				if(tri.intersection(r, t, u, v))
				{
					float depth = r(t).z;
					int depthBufferIndex = index(pixel.x, pixel.y);

					if(depth < m_depthBuffer[depthBufferIndex])
					{
						m_depthBuffer[depthBufferIndex] = depth;
						glm::vec3 color = m_texture.getColor((1 - v - u) * tri[0].uv + u * tri[1].uv + v * tri[2].uv);
						setPixelColor(x, y, color);
					}
				}
				else
				{
					//
				}
			}
		}
	stbi_write_png("output/cubeExample.png", m_image.width, m_image.height, m_image.channels, m_image.data, m_image.step);
}
