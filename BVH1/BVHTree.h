#include "AABB.h"
#include "BVHTreeNode.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#ifndef _BVH_TREE
#define _BVH_TREE

class BVHTree {
private:
	BVHTreeNode *root;
	std::unordered_map<std::string, BVHTreeNode *> map;
public:
	BVHTree();
	~BVHTree();
	void destroy(BVHTreeNode* node);
	void printNode(std::ostream &out, BVHTreeNode *, int level);
	void addBVHMember(AABB objectArea, std::string name);
	void moveBVHMember(std::string name, AABB newLocation);
	void removeBVHMember(std::string name);
	BVHTreeNode* getSibling(BVHTreeNode* node);
	void checkCollision(BVHTreeNode* node, AABB object, std::vector<std::string>& vec);
	std::vector<std::string> getCollidingObjects(AABB object);
	friend std::ostream &operator<<(std::ostream &out, BVHTree &tree);
};

#endif