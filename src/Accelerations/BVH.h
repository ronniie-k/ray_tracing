#pragma once

#include<vector>
#include<memory>

#include"Shape/AABB.h"
#include"Shape/Triangle.h"
#include"Shape/IntersectionInfo.h"

//references
//https://github.com/CMU-Graphics/Scotty3D/blob/main/src/rays/bvh.h
//https://cmu-graphics.github.io/Scotty3D/pathtracer/bounding_volume_hierarchy
//https://github.com/mmp/pbrt-v3/blob/master/src/accelerators/bvh.cpp

enum class SplitMethod { SpatialMedian, SortedMedian, SAH, Equal };

struct Node
{
	Node();
	Node(const AABB& bbox);
	AABB boundingBox;
	size_t start, end;
	std::shared_ptr<Node> left, right;
	int splitAxis;

	void addLeafNode(int offset, int numTris, const AABB& aabb);
	void addNode(std::shared_ptr<Node>& leftChild, std::shared_ptr<Node>& rightChild, int axis);
	bool isLeaf() const;
	friend class BVH;
};

struct LinearNode
{
	AABB boundingBox;
	union
	{
		int triOffset; //leaf
		int secondChildOffset; //interior
	};
	int numTris = 0;
	int splitAxis = 0;
};

class BVH
{
public:
	BVH(const std::vector<Triangle>& tris, SplitMethod method);

	bool intersect(Ray& r, IntersectionInfo& info);
private:
	std::shared_ptr<Node> build(std::vector<const Triangle*>& orderedTris, size_t start, size_t end);
	int flattenTree(std::shared_ptr<Node>& node, int& offset);

	SplitMethod m_method;
	std::vector<LinearNode> m_nodes;
	std::vector<const Triangle*> m_triangles;
	unsigned m_nodeCount = 0;
};

//todo
//sah, intersect
//kd tree