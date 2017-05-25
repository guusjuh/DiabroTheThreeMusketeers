#pragma once
#include <vector>
#include "City.h"

/// <summary>
/// A node from the grid used by A*.
/// </summary>
class Node
{
public:
	Node(Node parent, int parentId, int x, int y, int goalX, int goalY);
	Node(int x, int y, int goalX, int goalY);
	Node() { hasParent = false; }
	~Node();
	int parentId;
	bool hasParent;
	int x, y;
	int f();
	int g;//the movement cost to move from the starting point A to a given square on the grid, following the path generated to get there.
	int h;//the estimated movement cost 
	void changeParent(Node newParent, int newParentId);
	std::vector<Coordinate> getNeighbours(bool* pCollisionGrid, int pWidth);
};

