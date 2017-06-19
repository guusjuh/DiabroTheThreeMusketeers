#include "Node.h"
#include <vector>
#include "City.h"
#include "GameManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="Node"/> class.
/// </summary>
/// <param name="parent">The parent.</param>
/// <param name="parentId">The parents id.</param>
/// <param name="x">The x pos.</param>
/// <param name="y">The y pos.</param>
/// <param name="goalX">The goal x pos for this path.</param>
/// <param name="goalY">The goal y pos for this path.</param>
Node::Node(Node parent, int parentId, int x, int y, int goalX, int goalY, bool weighted) :
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

	if (weighted){
		int weight = 0;
		std::vector<Character*> hostileNpcs = GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs();
		for (size_t i = 0; i < hostileNpcs.size(); i++)
		{
			Coordinate npcPos = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(hostileNpcs[i]->getPosition().x, hostileNpcs[i]->getPosition().z));
			if (npcPos.x == x && npcPos.z == y){
				weight += 10;
			}
			if (((BaseNpc*)hostileNpcs[i])->stateMachine.getCurrentState() == "FollowAStar"){
				std::vector<Coordinate> nextPositions = ((BaseNpc*)hostileNpcs[i])->getNextPosList();
				for (size_t j = 0; j < nextPositions.size(); j++)
				{
					Coordinate nextPos = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getCollisionGridPosition(Coordinate(nextPositions[j].x, nextPositions[j].z));
					if (nextPos.x == x && nextPos.z == y){
						weight += 10;
					}
				}
			}
		}
		g += weight;
	}

	h = (std::abs(x - goalX) + std::abs(y - goalY)) * 10;
}

/// <summary>
/// Initializes a new instance of the <see cref="Node"/> class.
/// </summary>
/// <param name="x">The x pos.</param>
/// <param name="y">The y pos.</param>
/// <param name="goalX">The goal x pos for this path.</param>
/// <param name="goalY">The goal y pos for this path.</param>
Node::Node(int x, int y, int goalX, int goalY) :
x(x), y(y){
	hasParent = false;
	g = 0;
	h = (std::abs(x - goalX) + std::abs(y - goalY)) * 10;
}


/// <summary>
/// Finalizes an instance of the <see cref="Node"/> class.
/// </summary>
Node::~Node()
{
}

/// <summary>
/// Returns the f value of this node given the current path.
/// </summary>
/// <returns></returns>
int Node::f() {
	return g + h;
}

/// <summary>
/// Changes the parent.
/// </summary>
/// <param name="newParent">The new parent.</param>
/// <param name="newParentId">The new parents id.</param>
void Node::changeParent(Node newParent, int newParentId) {
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

/// <summary>
/// Gets the neighbours of this node.
/// </summary>
/// <param name="pCollisionGrid">The collision grid.</param>
/// <param name="pWidth">Width of the nodes in the grid.</param>
/// <returns></returns>
std::vector<Coordinate> Node::getNeighbours(bool* pCollisionGrid, int pWidth, int pHeight) {
	std::vector<Coordinate> neighbours;
	neighbours.clear();
	/*
	000
	0*x
	000
	*/
	if (x + 1 < pWidth){
		if (pCollisionGrid[x + 1 + y * pWidth])
		{
			neighbours.push_back(Coordinate(x + 1, y));
		}
	}
	/*
	000
	X*0
	000
	*/
	if (x - 1 >= 0){
		if (pCollisionGrid[x - 1 + y * pWidth])
		{
			neighbours.push_back(Coordinate(x - 1, y));
		}
	}
	/*
	000
	0*0
	0X0
	*/
	if (y + 1 < pHeight){
		if (pCollisionGrid[x + (y + 1) * pWidth])
		{
			neighbours.push_back(Coordinate(x, y + 1));
		}
	}
	/*
	0X0
	0*0
	000
	*/
	if (y - 1 >= 0){
		if (pCollisionGrid[x + (y - 1) * pWidth])
		{
			neighbours.push_back(Coordinate(x, y - 1));
		}
	}
	/*
	000
	0*0
	00X
	*/
	if (x + 1 < pWidth && y + 1 < pHeight){
		if (pCollisionGrid[x + 1 + (y + 1) * pWidth])
		{
			if (pCollisionGrid[x + 1 + y * pWidth] && pCollisionGrid[x + (y + 1) * pWidth])
			{
				neighbours.push_back(Coordinate(x + 1, y + 1));
			}
		}
	}
	/*
	000
	0*0
	X00
	*/
	if (x - 1 >= 0 && y + 1 < pHeight){
		if (pCollisionGrid[x - 1 + (y + 1) * pWidth])
		{
			if (pCollisionGrid[x - 1 + y * pWidth] && pCollisionGrid[x + (y + 1) * pWidth])
			{
				neighbours.push_back(Coordinate(x - 1, y + 1));
			}
		}
	}
	/*
	00X
	0*0
	000
	*/
	if (x + 1 < pWidth && y - 1 >= 0){
		if (pCollisionGrid[x + 1 + (y - 1) * pWidth])
		{
			if (pCollisionGrid[x + 1 + y * pWidth] && pCollisionGrid[x + (y - 1) * pWidth])
			{
				neighbours.push_back(Coordinate(x + 1, y - 1));
			}
		}
	}
	/*
	X00
	0*0
	000
	*/
	if (x - 1 >= 0 && y - 1 >= 0){
		if (pCollisionGrid[x - 1 + (y - 1) * pWidth])
		{
			if (pCollisionGrid[x - 1 + y * pWidth] && pCollisionGrid[x + (y - 1) * pWidth])
			{
				neighbours.push_back(Coordinate(x - 1, y - 1));
			}
		}
	}
	return neighbours;
}
