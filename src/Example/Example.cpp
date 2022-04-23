#include "Example.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb_image_write.h>

#include<glm/vec3.hpp>
#include<glm/geometric.hpp>
#include<glm/trigonometric.hpp>

#include"Random.h"
#include"Renderer/Models/Reflection.h"

#include"../Timer.h"

Example::Example(Image& img, const std::string& file, const glm::vec3& offset, int maxDepth, int nSamples)
	:m_image(img), m_model(file, offset), m_depth(maxDepth), m_nSamples(nSamples)
{
	for(unsigned y = 0; y < m_image.height * m_image.channels; y++)
		for(unsigned x = 0; x < m_image.width * m_image.channels; x++)
			m_image.data[index(x, y)] = 0;

	m_camera.position = {0, 0, 0};
	m_camera.target = {0, 0, 1};

	m_camera.lookAt = glm::normalize(m_camera.target - m_camera.position);
	m_camera.right = glm::normalize(glm::cross({0, 1, 0}, m_camera.lookAt));
	m_camera.up = glm::cross(m_camera.lookAt, m_camera.right);
}

int Example::index(int x, int y)
{
	return y * m_image.width + x;
}

Ray Example::getRayThroughPixel(float x, float y)
{
	//auto begin = std::chrono::high_resolution_clock::now();

	float fovScale = 1;// 1.428184f; //tan55
	glm::vec3 pixelToWorld(0.f);
	pixelToWorld += 2.f * (((x) / m_image.width) - 0.5f) * m_image.aspectRatio * fovScale * m_camera.right;
	pixelToWorld += -2.f * (((y) / m_image.height) - 0.5f) * fovScale * m_camera.up;
	pixelToWorld += -m_camera.position + m_camera.lookAt;

	//auto end = std::chrono::high_resolution_clock::now();
	//Timer::add("getRayThroughPixel()", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());

	return Ray(m_camera.position, glm::normalize(pixelToWorld));
}

void Example::setPixelColor(int x, int y, const glm::vec3& color)
{
	//auto begin = std::chrono::high_resolution_clock::now();

	int imageIndex = index(x * m_image.channels, y * m_image.channels);
	glm::vec3 srgb = srgbFromLrgb(color);
	m_image.data[imageIndex + 0] = static_cast<unsigned char>(255.f * srgb.r);
	m_image.data[imageIndex + 1] = static_cast<unsigned char>(255.f * srgb.g);
	m_image.data[imageIndex + 2] = static_cast<unsigned char>(255.f * srgb.b);

	//auto end = std::chrono::high_resolution_clock::now();
	//Timer::add("setPixelColor()", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
}

//https://en.wikipedia.org/wiki/SRGB
glm::vec3 Example::srgbFromLrgb(const glm::vec3& lrgb)
{
	//constants made static as it is faster than repeatedly constructing vec3s
	static glm::vec3 a(0.0031308f), b(12.92f), c(1.055f), d(1.f / 2.4f), e(0.055f);
	glm::vec3 isLower = glm::lessThan(lrgb, a);
	glm::vec3 lo = b * lrgb;
	glm::vec3 hi = c * glm::pow(lrgb, d) - e;

	return glm::mix(hi, lo, isLower);
}

glm::vec3 Example::tracePath(Ray& r, int depth, const Ray& previous)
{
	//auto begin = std::chrono::high_resolution_clock::now();

	static glm::vec3 white(1.f);
	static float p = 1 / 6.28318530718f;
	enum class isectType { sphere, tri, none };

	//i am aware of how bad the sphere/tri solution is from a software engineering perspective
	//a better solution would be polymorphism with a Shape base class
	//but there is no time for this, and my project will only have 2 shapes
	//so this solution is sufficient given the circumstances

	if(depth >= m_depth)
		return glm::vec3(0);

	glm::vec3 color(0);
	float tMin = std::numeric_limits<float>::infinity();

	isectType shape = isectType::none;
	Triangle* closestTri = nullptr;
	Sphere* closestSphere = nullptr;
	glm::vec3 surfaceNormal;
	Material mat;

	//get closest intersection
	for(Triangle& tri : m_model)
	{
		float u, v, t;

		if(tri.intersection(r, t, u, v))
		{
			if(t < tMin)
			{
				tMin = t;
				shape = isectType::tri;
				closestTri = &tri;
			}
		}
	}
	for(Sphere& s : m_spheres)
	{
		float t;
		s.intersection(r, t);
		if(t < tMin)
		{
			tMin = t;
			shape == isectType::sphere;
			closestSphere = &s;
		}
	}

	if(shape == isectType::none || tMin < 0 || tMin == std::numeric_limits<float>::infinity())
		return glm::vec3(0);

	glm::vec3 lightPos = {0, 0.85, -1.55};
	glm::vec3 intersectionPoint = r(tMin);

	if(shape == isectType::tri)
	{
		surfaceNormal = closestTri->getNormal();
		mat = closestTri->getMaterial();
	}
	else
	{
		surfaceNormal = closestSphere->getNormal(intersectionPoint);
		mat = m_model.getMaterial(0);
	}

	//rendering equation
	//reference: https://www.youtube.com/watch?v=eo_MTI-d28s
	//Lo(x, wo) = Le(x, wo) + ∫ fr(x, wi -> wo) * Li(x, wi) * wi.n dwi
	//(1) Lo is the color of a given pixel
		//x is the intersection point, wo is the vector that goes from x to the camera
	//(2) Le is the emitted radiance from x, this is 0 for all non-light sources, and the color of the light for ligh sources
	//(3) Li is radiance reflected from x, in the direction wi, this is the recursive part of the path tracing algorithm
	//(4) wi.n is self explanatory, cos(theta) = wi.n
	//(5) fr(x, wi->wo) is the brdf
	//(6) all of this is done under a hemisphere with respect with the normal of the surface which x lies on

	//generate secondary ray (6)
	Ray wi;

	//reference
	//https://gist.github.com/andrewbolster/10274979
	//generate random unit vector
	float phi = Random::getFloatInRange(0.f, 6.28318530718f); //phi = [0, 2pi]
	float costheta = Random::getFloatInRange(-1, 1);
	float theta = glm::acos(costheta);

	glm::vec3 dir(0.f);
	dir.x = glm::sin(theta) * glm::cos(phi);
	dir.y = glm::sin(theta) * glm::sin(phi);
	dir.z = costheta;

	//reference
	//https://math.stackexchange.com/questions/4010111/how-to-generate-a-random-vector-guaranteed-to-be-within-the-hemisphere-with-res
	if(glm::dot(surfaceNormal, dir) <= 0)
		dir *= -1;

	wi.direction = dir;
	wi.origin = intersectionPoint + 0.1f * dir;

	//(4)
	float dp = glm::dot(dir, surfaceNormal); //uses wi.dir

	//(2)
	glm::vec3 le = glm::vec3(0.f);
	if(mat.name == "light")
		le = {0.7, 0.7, 0.7};

	//brdf (5)
	float distance = glm::distance(lightPos, intersectionPoint);
	float attenuation = 1.f / (distance * distance);

	glm::vec3 lightVector = lightPos - intersectionPoint;
	glm::vec3 wo = m_camera.position - intersectionPoint; //here
	glm::vec3 brdf = BRDF::blinnPhong(lightVector, wo, surfaceNormal, mat) * attenuation;
	glm::vec3 abc = BRDF::blinnPhong(wi.direction, wo, surfaceNormal, mat) * glm::max(glm::dot(wi.direction, wo), 0.f) * 0.1f;
	abc = glm::min(white, abc);

	//(3)
	glm::vec3 li = abc + tracePath(wi, depth + 1) * dp;

	//(1)
	//multiply by p = 1 / 2π as that is the probability of any given ray
	//glm::vec3 lo = le + (li * brdf) * dp + brdfLight;
	glm::vec3 lo = (le + brdf + li);

	glm::vec3 shadowDir = glm::normalize(lightPos - intersectionPoint);
	Ray shadow(intersectionPoint + (0.1f * shadowDir), shadowDir);

	//auto end = std::chrono::high_resolution_clock::now();
	//Timer::add("tracePath()", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());

	if(inShadow(shadow, closestSphere, closestTri) && mat.name != "light")
		return abc + li;


	return glm::min(lo, white);
}

bool Example::inShadow(Ray& r, Sphere* sphere, Triangle* triangle)
{
	//auto begin = std::chrono::high_resolution_clock::now();

	for(Sphere& s : m_spheres)
	{
		if(&s == sphere)
			continue;
		float t;
		if(s.intersection(r, t))
			return true;
	}

	//auto end = std::chrono::high_resolution_clock::now();
	//Timer::add("inShadow()", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());

	return false;

	float tMin = std::numeric_limits<float>::infinity();
	Triangle* closest = nullptr;
	//get closest intersection
	for(Triangle& tri : m_model)
	{
		float u, v, t;

		if(tri.intersection(r, t, u, v))
		{
			if(t < tMin)
			{
				tMin = t;
				closest = &tri;
			}
		}
	}
	if(closest == nullptr)
		return false;

	if(closest->getMaterial().name == "light" || tMin <= 0)
		return false;


	return true;
}

void Example::writeImage(const std::string& output)
{
	std::string tmp = "output/" + output + ".png";
	stbi_write_png(tmp.c_str(), m_image.width, m_image.height, m_image.channels, m_image.data, m_image.step);
}
