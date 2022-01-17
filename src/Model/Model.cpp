#include "Model.h"

#include"Log.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include"tiny_obj_loader.h"

Model::Model(const std::string& filepath)
{
	std::string warn, error, directory;
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	int i = filepath.size();

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

	for(auto& shape : shapes)
	{
		for(auto& index : shape.mesh.indices)
		{
			Vertex v;
			if(index.vertex_index >= 0)
			{
				v.position.x = attrib.vertices[3 * index.vertex_index];
				v.position.y = attrib.vertices[3 * index.vertex_index + 1] - 1;
				v.position.z = 2 + attrib.vertices[3 * index.vertex_index + 2];
			}

			if(index.normal_index >= 0)
			{
				v.normal.x = attrib.normals[3 * index.normal_index];
				v.normal.y = attrib.normals[3 * index.normal_index + 1];
				v.normal.z = attrib.normals[3 * index.normal_index + 2];
			}

			if(index.texcoord_index >= 0)
			{
				v.uv.x = attrib.texcoords[2 * index.texcoord_index];
				v.uv.y = attrib.texcoords[2 * index.texcoord_index + 1];
			}
			m_vertices.push_back(v);
		}
	}
}
