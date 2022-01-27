#pragma once

#include"Shape.h"
#include"Vertex.h"
#include"Renderer/Ray.h"
#include"Model/Material.h"

#include<array>
#include<vector>

class Triangle : public Shape
{
public:
	Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2, std::vector<Material>& materials);
	Triangle(const std::array<Vertex, 3>& vertices, std::vector<Material>& materials);

	const Vertex& operator[] (size_t pos) const { return m_vertices[pos]; }
	Vertex& operator[] (size_t pos) { return m_vertices[pos]; }

	const Material& getMaterial();

	const glm::vec3& getNormal();
public:
	bool intersection(Ray& r, float& t, float& u, float& v) const; //moller trumbore
#if 0
	bool intersection(Ray& r, float& t); //half plane
	bool intersection(Ray& r, float& t, float& u, float& v); //barycentric
#endif
private:
	std::array<Vertex, 3> m_vertices;
	
	//--this may change in the future--
	//in a scene, every triangle must be part of a model
	//to create a cleaner interface i store a reference to the model's materials here
	std::vector<Material>& m_materials;
	int m_materialID; //i also assume that all vertices in a triangle share the same material properties
};