#include "Node.h"
#include <vector>
#include "City.h"

Node::Node(Node parent, int parentId, int x, int y, int goalX, int goalY) :
parentId(parentId), x(x), y(y)
{
	hasParent = true;
	if (x - parent.x != 0 && y - parent.y != 0)
	{
		g = parent.g + 14;
	}
	else
	{
		g = parent.g + 10;
	}
	h = (std::abs(x - goalX) + std::abs(y - goalY)) * 10;
}

Node::Node(int x, int y, int goalX, int goalY) : 
x(x), y(y){
	hasParent = false;
	g = 0;
	h = (std::abs(x - goalX) + std::abs(y - goalY)) * 10;
}


Node::~Node()
{
}

int Node::f(){
	return g + h;
}

void Node::changeParent(Node newParent, int newParentId){
	int newG;
	if (x - newParent.x != 0 && y - newParent.y != 0)
	{
		newG = newParent.g + 14;
	}
	else
	{
		newG = newParent.g + 10;
	}
	if (newG < g){
		parentId = newParentId;
	}
}

std::vector<Coordinate> Node::getNeighbours(bool* pCollisionGrid, int pWidth){
	std::vector<Coordinate> neighbours;
	neighbours.clear();
	/*
	000
	0*x
	000
	*/
	if (pCollisionGrid[x + 1 + y * pWidth])
	{
		neighbours.push_back(Coordinate(x + 1, y));
	}
	/*
	000
	X*0
	000
	*/
	if (pCollisionGrid[x - 1 + y * pWidth])
	{
		neighbours.push_back(Coordinate(x - 1, y));
	}
	/*
	000
	0*0
	0X0
	*/
	if (pCollisionGrid[x + (y + 1) * pWidth])
	{
		neighbours.push_back(Coordinate(x, y + 1));
	}
	/*
	0X0
	0*0
	000
	*/
	if (pCollisionGrid[x + (y - 1) * pWidth])
	{
		neighbours.push_back(Coordinate(x, y - 1));
	}
	/*
	000
	0*0
	00X
	*/
	if (pCollisionGrid[x + 1 + (y + 1) * pWidth])
	{
		if (pCollisionGrid[x + 1 + y * pWidth] && pCollisionGrid[x + (y + 1) * pWidth])
		{
			neighbours.push_back(Coordinate(x + 1, y + 1));
		}
	}
	/*
	000
	0*0
	X00
	*/
	if (pCollisionGrid[x - 1 + (y + 1) * pWidth])
	{
		if (pCollisionGrid[x - 1 + y * pWidth] && pCollisionGrid[x + (y + 1) * pWidth])
		{
			neighbours.push_back(Coordinate(x - 1, y + 1));
		}
	}
	/*
	00X
	0*0
	000
	*/
	if (pCollisionGrid[x + 1 + (y - 1) * pWidth])
	{
		if (pCollisionGrid[x + 1 + y * pWidth] && pCollisionGrid[x + (y - 1) * pWidth])
		{
			neighbours.push_back(Coordinate(x + 1, y - 1));
		}
	}
	/*
	X00
	0*0
	000
	*/
	if (pCollisionGrid[x - 1 + (y - 1) * pWidth])
	{
		if (pCollisionGrid[x - 1 + y * pWidth] && pCollisionGrid[x + (y - 1) * pWidth])
		{
			neighbours.push_back(Coordinate(x - 1, y - 1));
		}
	}
	return neighbours;
}
