#pragma once

#include"Vertex.h"
#include"Texture/Texture.h"

#include<vector>

class Model
{
public:
	Model(const std::string& filepath);

	std::vector<Vertex>::iterator begin() noexcept { return m_vertices.begin(); }
	std::vector<Vertex>::iterator end() noexcept { return m_vertices.end(); }
	const Vertex& operator[] (size_t pos) const { return m_vertices[pos]; }

	size_t size() const noexcept { return m_vertices.size(); }
private:
	//my stuff
	std::vector<Vertex> m_vertices;
	std::string m_texture; //this is temp for now, will need to figure out textures when i load more complex models
};