#include "Model.h"

#include"Log.h"
#include<glm/gtc/type_ptr.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include"tiny_obj_loader.h"

#include"../Instrumentor.h"

Model::Model(const std::string& filepath, const glm::vec3& offset)
{
	std::string warn, error, directory;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	int i = filepath.size();

	//get the directory/folder of the obj file
	for(; i > 0; i--)
		if(filepath[i] == '/')
		{
			directory = filepath.substr(0, i);
			break;
		}

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, filepath.c_str(), directory.c_str()))
	{
		Log::warn(warn);
		Log::error(error);
	}

	i = 0;

	for(auto& shape : shapes)
	{
		for(int i = 0; i < shape.mesh.indices.size(); i += 3)
		{
			std::array<Vertex, 3> verts;
			for(int k = 0; k < 3; k++)
			{
				Vertex v;
				v.materialID = shape.mesh.material_ids[i / 3];

				int vertexIndex = shape.mesh.indices[i + k].vertex_index;
				int normalIndex = shape.mesh.indices[i + k].normal_index;
				int texcoordIndex = shape.mesh.indices[i + k].texcoord_index;

				if(vertexIndex >= 0)
				{
					v.position.x = offset.x + attrib.vertices[3 * vertexIndex];
					v.position.y = offset.y + attrib.vertices[3 * vertexIndex + 1];
					v.position.z = offset.z + attrib.vertices[3 * vertexIndex + 2];
				}

				if(normalIndex >= 0)
				{
					v.normal.x = attrib.normals[3 * normalIndex];
					v.normal.y = attrib.normals[3 * normalIndex + 1];
					v.normal.z = attrib.normals[3 * normalIndex + 2];
				}

				if(texcoordIndex >= 0)
				{
					v.uv.x = attrib.texcoords[2 * texcoordIndex];
					v.uv.y = attrib.texcoords[2 * texcoordIndex + 1];
				}
				verts[k] = v;
			}
			m_triangles.emplace_back(verts, m_materials);
		}
	}

	for(auto& material : materials)
	{
		Material mat;
		mat.name = material.name;
		mat.ka = glm::make_vec3(material.ambient);
		mat.kd = glm::make_vec3(material.diffuse);
		mat.ks = glm::make_vec3(material.specular);
		mat.ns = material.shininess;
		mat.d = material.dissolve;
		m_materials.push_back(mat);
	}
}

Model::Model(Model& other)
	:m_triangles(other.m_triangles), m_materials(other.m_materials)
{
	Log::info("dd");
}

Model& Model::operator=(const Model& rhs)
{
	Log::info("d");
	if(this != &rhs)
	{
		m_triangles = rhs.m_triangles;
		m_materials = rhs.m_materials;
	}

	return *this;
}

Model::Model(Model&& other)
	:m_triangles(std::move(other.m_triangles)), m_materials(std::move(other.m_materials))
{
}

Model& Model::operator=(Model&& rhs)
{
	if(this != &rhs)
	{
		m_triangles = std::move(rhs.m_triangles);
		m_materials = std::move(rhs.m_materials);
	}

	return *this;
}
