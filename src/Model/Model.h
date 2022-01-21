#pragma once

#include"Vertex.h"
#include"Texture/Texture.h"
#include"Material.h"

#include<vector>
#include<unordered_map>

class Model
{
public:
	Model(const std::string& filepath, const glm::vec3& offset);

	std::vector<Vertex>::iterator begin() noexcept { return m_vertices.begin(); }
	std::vector<Vertex>::iterator end() noexcept { return m_vertices.end(); }
	const Vertex& operator[] (size_t pos) const { return m_vertices[pos]; }

	size_t size() const noexcept { return m_vertices.size(); }

	const Material& getMaterial(int id) { return m_materials[id]; }
private:
	//my stuff
	std::vector<Vertex> m_vertices;
	std::vector<Material> m_materials;
	std::string m_texture; //this is temp for now, will need to figure out textures when i load more complex models
};