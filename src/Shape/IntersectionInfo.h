#pragma once

#include"Triangle.h"

//intersection information
struct IntersectionInfo
{
	float t;
	float u, v; //barycentric coords for tris
	const Triangle* tri;
};