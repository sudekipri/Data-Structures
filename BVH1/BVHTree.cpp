#include "BVHTree.h"

BVHTree::BVHTree()
{
	root = nullptr;
}

BVHTree::~BVHTree()
{
	destroy(root);
}

void BVHTree::destroy(BVHTreeNode* node) 
{
	if(node) 
	{
		destroy(node->leftChild);
		destroy(node->rightChild);
		delete node;
	}
}

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}

void BVHTree::addBVHMember(AABB objectArea, std::string name)
{
	BVHTreeNode* newNode = new BVHTreeNode(objectArea, name, true);
	map[name] = newNode;

	if (!root) {				//Tree is empty
		root = newNode;
	}
	else if (root->isLeaf) {	//There is only one node
		auto oldRoot = root;
		root = new BVHTreeNode(objectArea + root->aabb, "branch", false);
		root->rightChild = oldRoot;
		root->leftChild = newNode;
		root->rightChild->parent = root;
		root->leftChild->parent = root;
	}
	else {						//There is more than one node
		auto branchNode = root;
		while (!branchNode->isLeaf) {
			int increaseInRight = AABB::unionArea(newNode->aabb, branchNode->rightChild->aabb) - branchNode->rightChild->aabb.getArea();
			int increaseInLeft = AABB::unionArea(newNode->aabb, branchNode->leftChild->aabb) - branchNode->leftChild->aabb.getArea();
			
			branchNode = increaseInRight < increaseInLeft ? branchNode->rightChild : branchNode->leftChild;
		}
		//branchNode is now existingLeaf
		auto newBranch = new BVHTreeNode(branchNode->aabb + objectArea, "branch", false);
		newBranch->rightChild = branchNode;
		newBranch->leftChild  = newNode;

		if (branchNode == branchNode->parent->rightChild) {
			branchNode->parent->rightChild = newBranch;
		}
		else {
			branchNode->parent->leftChild = newBranch;
		}

		newBranch->parent = branchNode->parent;
		newBranch->rightChild->parent = newBranch;
		newBranch->leftChild->parent  = newBranch;
		
		branchNode = newBranch->parent;
		do {
			branchNode->aabb = branchNode->aabb + objectArea;
			branchNode = branchNode->parent;
		} while (branchNode);
	}
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation)
{
	if (map.find(name) == map.end()) return;

	BVHTreeNode* selectedNode = map[name];
	if (!selectedNode->parent || !AABB::isCovered(newLocation, selectedNode->parent->aabb)) {
		removeBVHMember(name);
		addBVHMember(newLocation, name);
	}
	else {
		selectedNode->aabb = newLocation;
	}
}

void BVHTree::removeBVHMember(std::string name)
{
	if (map.find(name) == map.end()) return;
	BVHTreeNode* toRemove = map[name];

	if (toRemove->parent) {

		BVHTreeNode* sibling = getSibling(toRemove);
		BVHTreeNode* oldParent = sibling->parent;
		sibling->parent = oldParent->parent;

		if (sibling->parent->leftChild == oldParent)
			sibling->parent->leftChild = sibling;
		else
			sibling->parent->rightChild = sibling;

		delete oldParent;
	}

	delete toRemove;
	map.erase(name);
}

BVHTreeNode* BVHTree::getSibling(BVHTreeNode* node) {
	return node == node->parent->rightChild ? node->parent->leftChild : node->parent->rightChild;
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object)
{
	std::vector<std::string> objects;

	checkCollision(root, object, objects);
	
	return objects;
}

void BVHTree::checkCollision(BVHTreeNode* node, AABB object, std::vector<std::string>& vec) {

	if (node->aabb.collide(object)) {
		if (node->isLeaf) vec.push_back(node->name);
		else {
			checkCollision(node->leftChild, object, vec);
			checkCollision(node->rightChild, object, vec);
		}
	}
}

std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}