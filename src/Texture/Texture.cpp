#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include"Log.h"

Texture::Texture(const std::string& file)
{
	int width;
	int height;
	int channels;

	unsigned char* pixels = stbi_load(file.c_str(), &width, &height, &channels, 0);
	if(pixels == nullptr)
	{
		Log::error("failed to load image: {}", file);
		return;
	}

	m_data = Image(width, height, channels);
	m_data.data = pixels;
}

Texture::~Texture()
{
	delete[] m_data.data;
}

glm::vec3 Texture::getColor(const glm::vec2& uv) const
{
	//idea behind this equation:
	//turn barycentric coordinate (float value) to pixel coords (range of 0 to width or height)
	//multiply each component by number of channels for correct offset
	//use 2D to 1D index equation
	int x = (uv.x * m_data.width);
	int y = (uv.y * m_data.height);

	int index = (x * m_data.channels) + (y * m_data.channels) * m_data.width;
	int r = m_data.data[index];
	int g = m_data.data[index + 1];
	int b = m_data.data[index + 2];

	return glm::vec3(r, g, b);
}
