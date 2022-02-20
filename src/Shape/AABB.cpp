#include "AABB.h"

#include"Constants.h"

#include<utility>

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
    :minExtent(min), maxExtent(max)
{
}

bool AABB::intersection(Ray& r, float& tStart, float& tEnd) const
{
    tStart = 0.f;

    //if the ray is parallel to the bounding boxes axis and not within the extents then it will never intersect (see reference)
    if(r.direction.x > -Constants::epsilon && r.direction.x < Constants::epsilon && (r.origin.x < minExtent.x || r.origin.x > maxExtent.x))
        return false;

    if(r.direction.y > -Constants::epsilon && r.direction.y < Constants::epsilon && (r.origin.y < minExtent.y || r.origin.y > maxExtent.y))
        return false;

    if(r.direction.z > -Constants::epsilon && r.direction.z < Constants::epsilon && (r.origin.z < minExtent.z || r.origin.z > maxExtent.z))
        return false;

    float invX = 1.f / r.direction.x;
    float invY = 1.f / r.direction.y;
    float invZ = 1.f / r.direction.z;

    //get start and end ts for each individual axis
    tStart = (minExtent.x - r.origin.x) * invX;
    tEnd   = (maxExtent.x - r.origin.x) * invX;

    float tStartY = (minExtent.y - r.origin.y) * invY;
    float tEndY   = (maxExtent.y - r.origin.y) * invY;

    tStart = std::max(tStartY, tStart);
    tEnd = std::min(tEndY, tEnd);

    float tStartZ = (minExtent.z - r.origin.z) * invZ;
    float tEndZ   = (maxExtent.z - r.origin.z) * invZ;

    tStart = std::max(tStartZ, tStart);
    tEnd = std::min(tEndZ, tEnd);

    if(tStart > tEnd || tEnd < 0)
        return false;

    return true;

    //makes use of tmin which I dont use, also accounts for the case that r.origin is inside the AABB,
    //this is mainly as a note for the future if there is a bug, then this is the first likely solution
    
    if(tStart > 0.f)
        return true;
    else
        return true;
}

//reference: https://youtu.be/TrqK-atFfWY?t=1097 from the timestamp to 27:00