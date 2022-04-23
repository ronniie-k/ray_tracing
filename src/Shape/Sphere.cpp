#include "Sphere.h"

#include<glm/geometric.hpp>
#include<chrono>

#include"Constants.h"
#include"Timer.h"

Sphere::Sphere(const glm::vec3& center, float radius)
    :m_center(center), m_radius(radius)
{
}

const glm::vec3& Sphere::getNormal(const glm::vec3& point) const
{
    return point - m_center;
}

const glm::vec3& Sphere::getCenter() const
{
    return m_center;
}

float Sphere::getRadius() const
{
    return m_radius;
}


//ref
//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool Sphere::intersection(Ray& r, float& t)
{
    //auto begin = std::chrono::high_resolution_clock::now();

    float t0, t1;
    glm::vec3 oc = r.origin - m_center;
    float A = glm::dot(r.direction, r.direction);
    float B = 2.f * glm::dot(r.direction, oc);
    float C = glm::dot(oc, oc) - (m_radius * m_radius);

    float disc = (B * B) - (4 * A * C);

    if(disc < 0)
        return false;

    if(disc == 0)
    {
        t0 = -0.5f * (B / A);
        t1 = t0;
    }
    else
    {
        float q;
        if(B > 0)
            q = -0.5f * (B - glm::sqrt(disc));
        else
            q = -0.5f * (B + glm::sqrt(disc));

        t0 = q / A;
        t1 = C / q;
    }       

    if(t0 > t1)
        std::swap(t0, t1);

    t = t0;

    //auto end = std::chrono::high_resolution_clock::now();
    //Timer::add("sphereIntersection()", std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());

    if(t > -Constants::epsilon && t < Constants::epsilon)
        return false;
    
    return true;
}
