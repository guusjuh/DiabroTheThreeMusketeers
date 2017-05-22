#include "Node.h"
#include <iostream>

Node::Node(Node* parent, int x, int y, int goalX, int goalY) :
parent(parent), x(x), y(y)
{
	hasParent = true;
	if (x - parent->x != 0 && y - parent->y != 0)
	{
		g = parent->g + 14;
	}
	else
	{
		g = parent->g + 10;
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

void Node::changeParent(Node* newParent){
	int newG;
	if (x - newParent->x != 0 && y - newParent->y != 0)
	{
		newG = newParent->g + 14;
	}
	else
	{
		newG = newParent->g + 10;
	}
	if (newG < g){
		parent = newParent;
	}
}
