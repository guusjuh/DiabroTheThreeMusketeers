#pragma once
class Node
{
public:
	Node(Node* parent, int x, int y, int goalX, int goalY);
	Node(int x, int y, int goalX, int goalY);
	Node() {}
	~Node();
	Node* parent;
	int x, y;
	int f();
	int g;//the movement cost to move from the starting point A to a given square on the grid, following the path generated to get there.
	int h;//the estimated movement cost 
	void changeParent(Node* newParent);
};

