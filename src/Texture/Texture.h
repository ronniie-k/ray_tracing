#pragma once

#include<string>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>

#include"Image.h"

/*class Texture
{
public:
	//singleton class
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture& operator=(Texture&&) = delete;

	~Texture() = default;

	static Texture& get();
private:
	Texture() = default;
	class TextureObject
	{
	public:
		TextureObject(const std::string& file);
		~TextureObject() = default;
	private:
		Image m_data;
		static unsigned m_id;
	};
};*/

class Texture
{
public:
	Texture(const std::string& file);
	~Texture();

	//takes in barycentric coordinate (Pu + Qv + (1 - v - u)R)
	glm::vec3 getColor(const glm::vec2& uv);
private:
	Image m_data;
	static unsigned m_id;
};