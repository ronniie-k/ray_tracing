#include "BVH.h"

#include<glm/common.hpp>
#include<algorithm>

enum class Axis
{
	X, Y, Z
};

Node::Node()
{
    left = right = nullptr;
    start = end = 0;
}

Node::Node(const AABB& bbox)
	:boundingBox(bbox)
{
	left = right = nullptr;
	start = end = 0;
    splitAxis = 0;
}

void Node::addLeafNode(int offset, int numTris, const AABB& aabb)
{
    start = offset;
    end = offset + numTris;
    boundingBox = aabb;
    left = right = nullptr;
}

void Node::addNode(std::shared_ptr<Node>& leftChild, std::shared_ptr<Node>& rightChild, int axis)
{
	left = std::move(leftChild);
	right = std::move(rightChild);

	glm::vec3 min, max;
	min = glm::min(left->boundingBox.minExtent, right->boundingBox.minExtent);
	max = glm::max(left->boundingBox.maxExtent, right->boundingBox.maxExtent);
	boundingBox = AABB(min, max);
    splitAxis = axis;
}

bool Node::isLeaf() const
{
    return left == nullptr && right == nullptr;
}

BVH::BVH(const std::vector<Triangle>& tris, SplitMethod method)
	:m_method(method)
{
	m_triangles.reserve(tris.size());

	int i = 0;
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::infinity());

	for(auto& tri : tris)
	{
		const glm::vec3& centroid = tri.getCentroid();
		min = glm::min(min, centroid);
		max = glm::max(max, centroid);
        m_triangles.push_back(&tri);
	}

	std::vector<const Triangle*> orderedTris;
	orderedTris.reserve(tris.size());

    int offset = 0;
	auto root = build(orderedTris, 0, tris.size());
	m_triangles.swap(orderedTris);
    m_nodes.resize(m_nodeCount);
    flattenTree(root, offset);
}


//taken from https://github.com/mmp/pbrt-v3/blob/master/src/accelerators/bvh.cpp
//and adjusted to fit my code/interface
bool BVH::intersect(Ray& r, IntersectionInfo& info)
{
    if(m_nodes.empty())
        return false;

    bool hit = false;
    int toVisitOffset = 0;
    int currentNodeIndex = 0;
    bool negativeDir[3] = {r.direction.x < 0, r.direction.y < 0, r.direction.z < 0};
    int nodesToVisit[64];

    float start, end, tri;

    while(true)
    {
        const LinearNode* node = &m_nodes[currentNodeIndex];
        if(node->boundingBox.intersection(r, start, end))
        {
            if(node->numTris > 0)
            {
                for(int i = 0; i < node->numTris; i++)
                {
                    float u, v;
                    if(m_triangles[node->triOffset]->intersection(r, tri, u, v))
                    {
                        info.t = tri;
                        info.u = u;
                        info.v = v;
                        info.tri = m_triangles[node->triOffset];
                        hit = true;
                    }
                }
                if(toVisitOffset == 0)
                    break;
                currentNodeIndex = nodesToVisit[--toVisitOffset];
            }
            else
            {
                if(negativeDir[node->splitAxis])
                {
                    nodesToVisit[toVisitOffset++] = currentNodeIndex + 1;
                    currentNodeIndex = node->secondChildOffset;
                }
                else
                {
                    nodesToVisit[toVisitOffset++] = node->secondChildOffset;
                    currentNodeIndex += 1;
                }
            }
        }
        else
        {
            if(toVisitOffset == 0)
                break;
            currentNodeIndex = nodesToVisit[--toVisitOffset];
        }
    }
    return hit;
}

//taken from https://github.com/mmp/pbrt-v3/blob/master/src/accelerators/bvh.cpp
//and adjusted to fit my code/interface
std::shared_ptr<Node> BVH::build(std::vector<const Triangle*>& orderedTris, size_t start, size_t end)
{
    auto node = std::make_shared<Node>();
    m_nodeCount++;

    glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::infinity());

	//get bounding box which contains all tris
    for(int i = start; i < end; ++i)
    {
        const glm::vec3& triMax = m_triangles[i]->getBoundingBox().maxExtent;
        const glm::vec3& triMin = m_triangles[i]->getBoundingBox().minExtent;
        min = glm::min(min, triMin);
        max = glm::max(max, triMax);
    }

    int numTris = end - start;
    if(numTris == 1) 
    {
        //create leaf node
        int offset = orderedTris.size();
        for(int i = start; i < end; ++i) 
			orderedTris.push_back(m_triangles[i]);

        AABB bbox = AABB(min, max);
        node->addLeafNode(offset, numTris, bbox);
        return node;
    }
    else 
	{
        glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity());
        glm::vec3 max = glm::vec3(-std::numeric_limits<float>::infinity());

		//get bbox which contains all centroids
        for(int i = start; i < end; ++i)
        {
            const glm::vec3& centroid = m_triangles[i]->getCentroid();
            min = glm::min(min, centroid);
            max = glm::max(max, centroid);
        }

        glm::vec3 boundsDistances = glm::abs(min) + glm::abs(max); //get the distances between the extremes
        float maxDistance = glm::max(boundsDistances.z, glm::max(boundsDistances.x, boundsDistances.y)); //find largest

        //find the dimension where the distance between the extents is greatest
        int dim;
        for(dim = 0; dim < 3; dim++)
            if(boundsDistances[dim] == maxDistance)
                break;

        int mid = (start + end) * 0.5f;

		//if this conition is true the bbox has a volume of 0, meaning that all tris are stacked on top of each other
        if(max[dim] == min[dim]) 
        {
            int offset = orderedTris.size();
            for(int i = start; i < end; ++i) 
				orderedTris.push_back(m_triangles[i]);

            node->addLeafNode(offset, numTris, AABB(min, max));
            return node;
        }
        else 
		{
            switch(m_method) 
			{
                case SplitMethod::SpatialMedian: 
                {
					//can add/subtract half the maximum distance to get the midpoint
					float midPoint = min[dim] + (0.5 * maxDistance);
					//now assign tris depending on which side centroids are
					auto midPtr = std::partition(&m_triangles[start], &m_triangles[end - 1] + 1,
												  [dim, midPoint](const Triangle*& tri)
												  {
													  return tri->getCentroid()[dim] <= midPoint;
												  });

					mid = midPtr - &m_triangles[0];
					if(mid != start && mid != end)
						break;
                }
                case SplitMethod::SortedMedian:
                {
                    mid = (start + end) * 0.5f;
                    std::nth_element(&m_triangles[start], &m_triangles[mid], &m_triangles[end - 1] + 1,
                    [dim](const Triangle*& a, const Triangle*& b)
                    {
                        return a->getCentroid()[dim] < b->getCentroid()[dim];
                    });
                }
				default:
				    break;
            }
            node->addNode(build(orderedTris, start, mid), build(orderedTris, mid, end), dim);
        }
    }
    return node;
}

int BVH::flattenTree(std::shared_ptr<Node>& node, int& offset)
{
    LinearNode* linearNode = &m_nodes[offset];
    linearNode->boundingBox = node->boundingBox;
    int myOffset = offset++;
    int numTris = node->end - node->start;
    if(numTris > 0)
    {
        linearNode->triOffset = node->start;
        linearNode->numTris = numTris;
    }
    else
    {
        linearNode->numTris = 0;
        flattenTree(node->left, offset);
        linearNode->secondChildOffset = flattenTree(node->right, offset);
    }
    return myOffset;
}
