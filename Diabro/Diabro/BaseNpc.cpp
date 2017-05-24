#include "BaseNpc.h"
#include "GameManager.h"
#include "Node.h" 

/// <summary>
/// Creates a new instance of the <see cref="BaseNpc" /> class.
/// All NPC's inherit from this class, either friendly and hostile NPC's.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
BaseNpc::BaseNpc(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity) : Character(pMyNode, pMyEntity), _timer(0), _noticeDistance(400.0f), _myCity(pMyCity)
{
	_myRotationNode = pMyRotationNode;
	goalPos = Coordinate(0, 0);
	//wander();
}

/// <summary>
/// Updates the frame based on the specified pDeltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void BaseNpc::update(Ogre::Real pDeltatime)
{
	//Character::update(pDeltatime);

	//detectPlayer();
	if (goalPos.x == 0 && goalPos.z == 0){
		calculateAStar(_myCity->getRandomPointInRoom());
	}

	_myNode->translate(_dirVec * getSpeed() * pDeltatime, Ogre::Node::TS_LOCAL);
	//if (!_playerDetected) {
		if (getPosition().distance(Ogre::Vector3(goalPos.x, getPosition().y, goalPos.z)) < 50){
			if (nextPos.size() == 0){
				calculateAStar(_myCity->getRandomPointInRoom());
			}
			else{
				walkToNextPoint();
			}
		}
	//}
}

/// <summary>
/// Rotates the pivot (same as rotating the model, doesnt affect movement).
/// </summary>
/// <param name="pRotationDegrees">The rotation degrees.</param>
void BaseNpc::rotatePivot(Ogre::Vector3 pRotationDegrees) {
	_myRotationNode->pitch(Ogre::Degree(pRotationDegrees.x), Ogre::Node::TS_LOCAL);
	_myRotationNode->yaw(Ogre::Degree(pRotationDegrees.y), Ogre::Node::TS_LOCAL);
	_myRotationNode->roll(Ogre::Degree(pRotationDegrees.z), Ogre::Node::TS_LOCAL);
}

/// <summary>
/// Detects the player if he is in range.
/// </summary>
void BaseNpc::detectPlayer()
{
	if (getPosition().distance(GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getPosition()) < _noticeDistance) {
		_playerDetected = true;
	}
	else {
		_playerDetected = false;
	}
}

/// <summary>
/// Lets this instance wander randomly.
/// </summary>
void BaseNpc::wander() 
{
	//Coordinate coord = _myCity->getRandomPoint();
	//walkTo(Ogre::Vector3(coord.x, getPosition().y, coord.z));
}

void BaseNpc::walkToNextPoint(){
	goalPos = nextPos[nextPos.size() - 1];
	nextPos.pop_back();
	_myNode->lookAt(Ogre::Vector3(goalPos.x, getPosition().y, goalPos.z), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
	//_myNode->setPosition(goalPos.x, _myNode->getPosition().y, goalPos.z);

	_dirVec = Ogre::Vector3(1, 0, 0);
}

void BaseNpc::walkTo(Ogre::Vector3 targetPos) 
{
	_myNode->lookAt(Ogre::Vector3(targetPos.x, getPosition().y, targetPos.z), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
	_dirVec = Ogre::Vector3(1, 0, 0);
}

void BaseNpc::calculateAStar(Ogre::Vector3 targetPos){
	//get the zoe from the levelmanager
	Zone* zone = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZonePointer(0, 0);

	//get the pointer to the collisiongrid
	bool* collisionGrid = zone->getCollisionGrid();
	//instantiate a second ersion of the collisiongrid
	bool* grid = new bool[zone->_width * zone->_depth];
	for (int ix = 0; ix < zone->_width; ++ix) {
		for (int iy = 0; iy < zone->_depth; ++iy) {
			grid[ix + iy * zone->_width] = collisionGrid[ix + iy * zone->_width];
		}
	}
	//get scale 
	int scale = _myCity->scalar;

	//check if target position is viable
	if (!collisionGrid[(int)(targetPos.x + (targetPos.z * zone->_width))]){
		//new random point in room
		Ogre::Vector3 coord = _myCity->getRandomPointInRoom();
		calculateAStar(Ogre::Vector3(coord.x, getPosition().y, coord.z));
		//stops method
		return;
	}

	std::vector<Node> nodes;	//stores al nodes
	std::vector<int> openList;	//stores the id's from the open nodes
	std::vector<int> closedList;//stores the id's from the closed nodes

	//make all vectors empty
	nodes.clear();
	openList.clear();
	closedList.clear();

	//get the current position of the npc
	Coordinate currentPos = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(getPosition().x, getPosition().z));

	//adds a start node to the nodelist(grid point at current position
	Node start = Node(currentPos.x, currentPos.z, targetPos.x, targetPos.z);
	nodes.push_back(start);
	openList.push_back(nodes.size() - 1);
	grid[start.x + start.y * zone->_width] = false;

	//last node from the path
	bool targetAdded = false;
	int count = 0;
	while (!targetAdded){
		count++;
		int lowestF = 1000000000;
		Node lowestFNode;
		int lowestFNodeId = -1;
		int pos;
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (nodes[openList[i]].f() < lowestF){
				lowestF = nodes[openList[i]].f();
				lowestFNode = nodes[openList[i]];
				lowestFNodeId = openList[i];
				pos = i;
			}
		}
		if (lowestFNodeId < 0){
			return;
		}
		//add the lowestf i't's ID to the closedList
		closedList.push_back(lowestFNodeId);

		//remove the closed node from the openlist
		openList.erase(openList.begin() + pos);

		//last node found
		if (lowestFNode.x == (int)targetPos.x && lowestFNode.y == (int)targetPos.z){
			//build up the nextPos list
			bool nextIteration = true;
			while (nextIteration){
				nextPos.push_back(Coordinate(lowestFNode.x * scale, lowestFNode.y * scale));
				if (lowestFNode.hasParent){
					//make the lowestFNode become its parent
					lowestFNode = nodes[lowestFNode.parentId];
				}
				else
				{
					//start node was reached end the loop
					nextIteration = false;
				}
			}
			//start the walking
			walkToNextPoint();
			return;
		}

		//add its neigbours to the openlist

		// o o o
		// x O o
		// o o o
		if (lowestFNode.x - 1 >= 0){
			if (collisionGrid[lowestFNode.x - 1 + (lowestFNode.y) * zone->_width]){
				if (grid[lowestFNode.x - 1 + (lowestFNode.y) * zone->_width]){
					grid[lowestFNode.x - 1 + (lowestFNode.y) * zone->_width] = false;
					nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x - 1, lowestFNode.y, targetPos.x, targetPos.z));
					openList.push_back(nodes.size() - 1);
				}
				else
				{
					for (size_t i = 0; i < openList.size(); i++)
					{
						if (nodes[openList[i]].x == lowestFNode.x - 1 && nodes[openList[i]].y == lowestFNode.y){
							nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
						}
					}
				}
			}
		}

		// o o o
		// o O x
		// o o o
		if (lowestFNode.x + 1 <= zone->_width){
			if (collisionGrid[lowestFNode.x + 1 + (lowestFNode.y) * zone->_width]){
				if (grid[lowestFNode.x + 1 + (lowestFNode.y) * zone->_width]){
					grid[lowestFNode.x + 1 + (lowestFNode.y) * zone->_width] = false;
					nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x + 1, lowestFNode.y, targetPos.x, targetPos.z));
					openList.push_back(nodes.size() - 1);
				}
				else
				{
					for (size_t i = 0; i < openList.size(); i++)
					{
						if (nodes[openList[i]].x == lowestFNode.x + 1 && nodes[openList[i]].y == lowestFNode.y){
							nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
						}
					}
				}
			}
		}

		// o x o
		// o O o
		// o o o
		if (lowestFNode.y - 1 >= 0){
			if (collisionGrid[lowestFNode.x + (lowestFNode.y - 1) * zone->_width]){
				if (grid[lowestFNode.x + (lowestFNode.y - 1) * zone->_width]){
					grid[lowestFNode.x + (lowestFNode.y - 1) * zone->_width] = false;
					nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x, lowestFNode.y - 1, targetPos.x, targetPos.z));
					openList.push_back(nodes.size() - 1);
				}
				else
				{
					for (size_t i = 0; i < openList.size(); i++)
					{
						if (nodes[openList[i]].x == lowestFNode.x && nodes[openList[i]].y == lowestFNode.y - 1){
							nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
						}
					}
				}
			}
		}

		// o o o
		// o O o
		// o x o
		if (lowestFNode.y + 1 <= zone->_depth){
			if (collisionGrid[lowestFNode.x + (lowestFNode.y + 1) * zone->_width]){
				if (grid[lowestFNode.x + (lowestFNode.y + 1) * zone->_width]){
					grid[lowestFNode.x + (lowestFNode.y + 1) * zone->_width] = false;
					nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x, lowestFNode.y + 1, targetPos.x, targetPos.z));
					openList.push_back(nodes.size() - 1);
				}
				else
				{
					for (size_t i = 0; i < openList.size(); i++)
					{
						if (nodes[openList[i]].x == lowestFNode.x && nodes[openList[i]].y == lowestFNode.y + 1){
							nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
						}
					}
				}
			}
		}

		//diagonal
		// x o o
		// o O o
		// o o o
		if (lowestFNode.x - 1 >= 0 && lowestFNode.y - 1 >= 0){
			if (collisionGrid[lowestFNode.x - 1 + (lowestFNode.y - 1) * zone->_width])
			{
				if (collisionGrid[lowestFNode.x + (lowestFNode.y - 1) * zone->_width] && collisionGrid[lowestFNode.x - 1 + (lowestFNode.y) * zone->_width]){
					if (grid[lowestFNode.x - 1 + (lowestFNode.y - 1) * zone->_width]){
						grid[lowestFNode.x - 1 + (lowestFNode.y - 1) * zone->_width] = false;
						nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x - 1, lowestFNode.y - 1, targetPos.x, targetPos.z));
						openList.push_back(nodes.size() - 1);
					}
					else
					{
						for (size_t i = 0; i < openList.size(); i++)
						{
							if (nodes[openList[i]].x == lowestFNode.x - 1 && nodes[openList[i]].y == lowestFNode.y - 1){
								nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
							}
						}
					}
				}
			}
		}

		// o o o
		// o O o
		// x o o
		if (lowestFNode.x - 1 >= 0 && lowestFNode.y + 1 <= zone->_depth){
			if (collisionGrid[lowestFNode.x - 1 + (lowestFNode.y + 1) * zone->_width])
			{
				if (collisionGrid[lowestFNode.x + (lowestFNode.y + 1) * zone->_width] && collisionGrid[lowestFNode.x - 1 + (lowestFNode.y) * zone->_width]){
					if (grid[lowestFNode.x - 1 + (lowestFNode.y + 1) * zone->_width]){
						grid[lowestFNode.x - 1 + (lowestFNode.y + 1) * zone->_width] = false;
						nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x - 1, lowestFNode.y + 1, targetPos.x, targetPos.z));
						openList.push_back(nodes.size() - 1);
					}
					else
					{
						for (size_t i = 0; i < openList.size(); i++)
						{
							if (nodes[openList[i]].x == lowestFNode.x - 1 && nodes[openList[i]].y == lowestFNode.y + 1){
								nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
							}
						}
					}
				}
			}
		}

		// o o x
		// o O o
		// o o o
		if (lowestFNode.x + 1 >= 0 && lowestFNode.y - 1 >= 0){
			if (collisionGrid[lowestFNode.x + 1 + (lowestFNode.y - 1) * zone->_width])
			{
				if (collisionGrid[lowestFNode.x + (lowestFNode.y - 1) * zone->_width] && collisionGrid[lowestFNode.x + 1 + (lowestFNode.y) * zone->_width]){
					if (grid[lowestFNode.x + 1 + (lowestFNode.y - 1) * zone->_width]){
						grid[lowestFNode.x + 1 + (lowestFNode.y - 1) * zone->_width] = false;
						nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x + 1, lowestFNode.y - 1, targetPos.x, targetPos.z));
						openList.push_back(nodes.size() - 1);
					}
					else
					{
						for (size_t i = 0; i < openList.size(); i++)
						{
							if (nodes[openList[i]].x == lowestFNode.x + 1 && nodes[openList[i]].y == lowestFNode.y - 1){
								nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
							}
						}
					}
				}
			}
		}

		// o o o
		// o O o
		// o o x
		if (lowestFNode.x + 1 >= 0 && lowestFNode.y + 1 <= zone->_depth){
			if (collisionGrid[lowestFNode.x + 1 + (lowestFNode.y + 1) * zone->_width])
			{
				if (collisionGrid[lowestFNode.x + (lowestFNode.y + 1) * zone->_width] && collisionGrid[lowestFNode.x + 1 + (lowestFNode.y) * zone->_width]){
					if (grid[lowestFNode.x + 1 + (lowestFNode.y + 1) * zone->_width]){
						grid[lowestFNode.x + 1 + (lowestFNode.y + 1) * zone->_width] = false;
						nodes.push_back(Node(lowestFNode, lowestFNodeId, lowestFNode.x + 1, lowestFNode.y + 1, targetPos.x, targetPos.z));
						openList.push_back(nodes.size() - 1);
					}
					else
					{
						for (size_t i = 0; i < openList.size(); i++)
						{
							if (nodes[openList[i]].x == lowestFNode.x + 1 && nodes[openList[i]].y == lowestFNode.y + 1){
								nodes[openList[i]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
							}
						}
					}
				}
			}
		}

		//check if openlist is empty
		if (openList.size() == 0){
			//break out of the loop target can't be found
			return;
		}
	}
}