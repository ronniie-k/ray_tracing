#pragma once

//base class used for all rendering examples
//e.g. rendering a cube, cornell box,
//cornell box using blinn phong, etc...

#include"Texture/Image.h"
#include"Renderer/Ray.h"
#include"Renderer/Camera.h"
#include"Renderer/Light.h"

#include"Model/Model.h"
#include"Shape/Sphere.h"

#include"Random.h"

class Example
{
public:
	Example(Image& img, const std::string& file, const glm::vec3& offset, int maxDepth, int nSamples);

	virtual void draw() = 0;
protected:
	int index(int x, int y);
	Ray getRayThroughPixel(float x, float y);

	void setPixelColor(int x, int y, const glm::vec3& color);
	glm::vec3 srgbFromLrgb(const glm::vec3& lrgb);

	glm::vec3 tracePath(Ray& r, int depth, const Ray& previous = {{0, 0, 0}, {0, 0, 0}});
	bool inShadow(Ray& r, Sphere* sphere = nullptr, Triangle* triangle = nullptr);

	void writeImage(const std::string& output);
protected:
	Camera m_camera;
	Image& m_image;

	Model m_model;
	std::vector<Sphere> m_spheres;
	std::vector<Light> m_lights;

	int m_depth = 1;
	int m_nSamples = 1;
};