#include "BaseNpc.h"
#include "GameManager.h"
#include "Node.h" 

/// <summary>
/// Creates a new instance of the <see cref="BaseNpc" /> class.
/// All NPC's inherit from this class, either friendly and hostile NPC's.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
BaseNpc::BaseNpc(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity) : Character(pMyNode, pMyEntity), _timer(0), _myCity(pMyCity)
{
	_myRotationNode = pMyRotationNode;
	goalPos = Coordinate(0, 0);
}

/// <summary>
/// Updates the frame based on the specified pDeltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void BaseNpc::update(Ogre::Real pDeltatime)
{
	//Character::update(pDeltatime);

	detectPlayer();
	//for initialization
	//TODO: use -1 -1 as values
	if (goalPos.x == 0 && goalPos.z == 0){
		calculateAStar(_myCity->getRandomPointInRoom());
	}

	_myNode->translate(_dirVec * getSpeed() * pDeltatime, Ogre::Node::TS_LOCAL);
	if (!_playerDetected) {
		if (getPosition().distance(Ogre::Vector3(goalPos.x, getPosition().y, goalPos.z)) < 50){
			if (nextPos.size() == 0){
				Ogre::Vector3 pos = Ogre::Vector3(_myCity->getRandomPointInRoom());
				Ogre::Vector3 goalPosition = Ogre::Vector3(goalPos.x / _myCity->Scalar(), getPosition().y, goalPos.z / _myCity->Scalar());
				while(goalPosition.distance(pos) < 1) {
					pos = Ogre::Vector3(_myCity->getRandomPointInRoom());
				}
				calculateAStar(pos);
			}
			else{
				walkToNextPoint();
			}
		}
	}
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
/// Makes the BaseNPC walk to next point.
/// </summary>
void BaseNpc::walkToNextPoint() {
	goalPos = nextPos[nextPos.size() - 1];
	nextPos.pop_back();
	_myNode->lookAt(Ogre::Vector3(goalPos.x, getPosition().y, goalPos.z), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);

	_dirVec = Ogre::Vector3(1, 0, 0);
}

/// <summary>
/// Makes the BaseNPC walk to the point.
/// </summary>
/// <param name="targetPos">The target position.</param>
void BaseNpc::walkTo(Ogre::Vector3 targetPos)
{
	_myNode->lookAt(Ogre::Vector3(targetPos.x, getPosition().y, targetPos.z), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
	_dirVec = Ogre::Vector3(1, 0, 0);
}

/// <summary>
/// Calculates the A* route from the current to the target position.
/// </summary>
/// <param name="targetPos">The target position.</param>
void BaseNpc::calculateAStar(Ogre::Vector3 targetPos) {
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
	int scale = _myCity->Scalar();

	//check if target position is viable
	if (!collisionGrid[(int)(targetPos.x + (targetPos.z * zone->_width))]){
		//debug("The target position is not a valid point in the collision gird\n");
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

	if (currentPos.x == targetPos.x && currentPos.z == targetPos.z){
		//debug("The goal is the same as the current position\n");
		//new random point in room
		Ogre::Vector3 coord = _myCity->getRandomPointInRoom();
		calculateAStar(Ogre::Vector3(coord.x, getPosition().y, coord.z));
		//stops method
		return;
	}

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
		std::vector<Coordinate> neighboursPositions = lowestFNode.getNeighbours(collisionGrid, zone->_width);
		for (size_t i = 0; i < neighboursPositions.size(); i++)
		{
			int pX = neighboursPositions[i].x;
			int pY = neighboursPositions[i].z;
			if (grid[pX + pY * zone->_width]){
				grid[pX + pY * zone->_width] = false;
				nodes.push_back(Node(lowestFNode, lowestFNodeId, pX, pY, targetPos.x, targetPos.z));
				openList.push_back(nodes.size() - 1);
			}
			else
			{
				for (size_t j = 0; j < openList.size(); j++)
				{
					if (nodes[openList[j]].x == pX && nodes[openList[j]].y == pY){
						nodes[openList[j]].changeParent(lowestFNode, lowestFNodeId);//checks for more efficient parent
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

/// <summary>
/// Debugs the specified message.
/// </summary>
/// <param name="msg">The message.</param>
/// <param name="val">The value.</param>
void BaseNpc::debug(std::string msg, int val) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("%s (%d)\n", msg.c_str(), val);
	fclose(fp);
#endif
}