#include "BaseNpc.h"
#include "GameManager.h"
#include "Node.h" 
#include "Debug.h"

const Ogre::String BaseNpc::_questMaterial = "InGame/HasQuest";
const Ogre::String BaseNpc::_activeMaterial = "InGame/ActiveInQuest";

/// <summary>
/// Creates a new instance of the <see cref="BaseNpc" /> class.
/// All NPC's inherit from this class, either friendly and hostile NPC's.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
BaseNpc::BaseNpc(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity) 
: Character(pMyNode, pMyEntity), _timer(0), _myCity(pMyCity), _inDialog(false), _isDead(false)
{
	// create the entity for the quest indicator
	questIndicatorEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	indicatorNode = pMyNode->createChildSceneNode();
	indicatorNode->attachObject(questIndicatorEntity);
	questIndicatorEntity->setMaterialName(_questMaterial);
	indicatorNode->setVisible(false);

	_degreePerFrame = 0;

	_forwardVec = Ogre::Vector3(-1, 0, 0);

	_myRotationNode = pMyRotationNode;
	goalPos = Coordinate(0, 0);
}

void BaseNpc::collide(){
	//stateMachine.collide();
}

/// <summary>
/// Updates the frame based on the specified pDeltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void BaseNpc::update(Ogre::Real pDeltatime)
{
	if (_dirVec != Ogre::Vector3(0, 0, 0)) {
		_dirVec = Ogre::Vector3(1, 0, 0);
	}
	detectPlayer();

	stateMachine.update();

	if(stateMachine.getCurrentState() != "Idle" && stateMachine.getCurrentState() != "Attack") {
		_wantedRotationAngle = angleBetween(Ogre::Vector3(goalPos.x, getPosition().y, goalPos.z));

		_degreePerFrame = _rotationspeed * pDeltatime;

		if (Ogre::Math::Abs(_wantedRotationAngle) > _degreePerFrame) {
			if (_wantedRotationAngle < 0) _degreePerFrame *= -1;
			_myNode->yaw(Ogre::Radian(Ogre::Math::DegreesToRadians(_degreePerFrame)), Ogre::Node::TS_LOCAL);
		}
		else {
			_myNode->yaw(Ogre::Radian(Ogre::Math::DegreesToRadians(_wantedRotationAngle)), Ogre::Node::TS_LOCAL);
		}
	}

	Character::update(pDeltatime);
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
	if (getPosition().distance(GameManager::getSingletonPtr()->getPlayer()->getPosition()) < _noticeDistance) {
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
	if (nextPos.size() == 0){
		_dirVec = Ogre::Vector3().ZERO;
		return;
	}
	goalPos = nextPos[nextPos.size() - 1];
	nextPos.pop_back();

	_dirVec = Ogre::Vector3(1, 0, 0);
}

/// <summary>
/// Makes the BaseNPC walk to the point.
/// </summary>
/// <param name="targetPos">The target position.</param>
void BaseNpc::walkTo(Ogre::Vector3 targetPos)
{
	goalPos = Coordinate(targetPos.x, targetPos.z);

	_dirVec = Ogre::Vector3(1, 0, 0);
}

/// <summary>
/// Calculates the A* route from the current to the target position.
/// </summary>
/// <param name="targetPos">The target position.</param>
void BaseNpc::calculateAStarWeighted(Ogre::Vector3 targetPos) {
	//get the zone from the levelmanager
	Zone* zone = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getZonePointer(0, 0);

	//get scale 
	int scale = _myCity->Scalar() / _myCity->gridScalar;

	//get the pointer to the collisiongrid
	bool* collisionGrid = zone->getCollisionGrid();
	//instantiate a second ersion of the collisiongrid
	bool* grid = new bool[zone->getResolution().x * _myCity->gridScalar * zone->getResolution().z * _myCity->gridScalar];
	for (int ix = 0; ix < zone->getResolution().x * _myCity->gridScalar; ++ix) {
		for (int iy = 0; iy < zone->getResolution().z * _myCity->gridScalar; ++iy) {
			grid[ix + iy * zone->getResolution().x * _myCity->gridScalar] = collisionGrid[ix + iy * zone->getResolution().x * _myCity->gridScalar];
		}
	}

	//check if target position is viable
	if (!collisionGrid[(int)(targetPos.x + (targetPos.z * zone->getResolution().x * _myCity->gridScalar))]){
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
	Coordinate currentPos = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(getPosition().x, getPosition().z));

	if (currentPos.x == targetPos.x && currentPos.z == targetPos.z) {
		//stops method
		return;
	}

	//adds a start node to the nodelist(grid point at current position
	Node start = Node(currentPos.x, currentPos.z, targetPos.x, targetPos.z);
	nodes.push_back(start);
	openList.push_back(nodes.size() - 1);
	grid[start.x + start.y * zone->getResolution().x * _myCity->gridScalar] = false;

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
			nextPos.clear();
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
		std::vector<Coordinate> neighboursPositions = lowestFNode.getNeighbours(collisionGrid, zone->getResolution().x * _myCity->gridScalar, zone->getResolution().z * _myCity->gridScalar);
		for (size_t i = 0; i < neighboursPositions.size(); i++)
		{
			int pX = neighboursPositions[i].x;
			int pY = neighboursPositions[i].z;
			if (grid[pX + pY * zone->getResolution().x * _myCity->gridScalar]){
				grid[pX + pY * zone->getResolution().x * _myCity->gridScalar] = false;
				nodes.push_back(Node(lowestFNode, lowestFNodeId, pX, pY, targetPos.x, targetPos.z, true));
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
/// Calculates the A* route from the current to the target position.
/// </summary>
/// <param name="targetPos">The target position.</param>
void BaseNpc::calculateAStar(Ogre::Vector3 targetPos) {
	//get the zone from the levelmanager
	Zone* zone = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getZonePointer(0, 0);

	//get scale 
	int scale = _myCity->Scalar() / _myCity->gridScalar;

	//get the pointer to the collisiongrid
	bool* collisionGrid = zone->getCollisionGrid();
	//instantiate a second ersion of the collisiongrid
	bool* grid = new bool[zone->getResolution().x * _myCity->gridScalar * zone->getResolution().z * _myCity->gridScalar];
	for (int ix = 0; ix < zone->getResolution().x * _myCity->gridScalar; ++ix) {
		for (int iy = 0; iy < zone->getResolution().z * _myCity->gridScalar; ++iy) {
			grid[ix + iy * zone->getResolution().x * _myCity->gridScalar] = collisionGrid[ix + iy * zone->getResolution().x * _myCity->gridScalar];
		}
	}

	//check if target position is viable
	if (!collisionGrid[(int)(targetPos.x + (targetPos.z * zone->getResolution().x * _myCity->gridScalar))]){
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
	Coordinate currentPos = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(getPosition().x, getPosition().z));

	if (currentPos.x == targetPos.x && currentPos.z == targetPos.z) {
		//stops method
		return;
	}

	//adds a start node to the nodelist(grid point at current position
	Node start = Node(currentPos.x, currentPos.z, targetPos.x, targetPos.z);
	nodes.push_back(start);
	openList.push_back(nodes.size() - 1);
	grid[start.x + start.y * zone->getResolution().x * _myCity->gridScalar] = false;

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
			nextPos.clear();
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
		std::vector<Coordinate> neighboursPositions = lowestFNode.getNeighbours(collisionGrid, zone->getResolution().x * _myCity->gridScalar, zone->getResolution().z * _myCity->gridScalar);
		for (size_t i = 0; i < neighboursPositions.size(); i++)
		{
			int pX = neighboursPositions[i].x;
			int pY = neighboursPositions[i].z;
			if (grid[pX + pY * zone->getResolution().x * _myCity->gridScalar]){
				grid[pX + pY * zone->getResolution().x * _myCity->gridScalar] = false;
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
void BaseNpc::walkToNeighbour(){
	//get the zoe from the levelmanager
	Zone* zone = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getZonePointer(0, 0);
	//get the pointer to the collisiongrid
	bool* collisionGrid = zone->getCollisionGrid();

	int scale = _myCity->Scalar() / _myCity->gridScalar;

	Coordinate currentPos = GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(getPosition().x, getPosition().z));
	Node start = Node(currentPos.x, currentPos.z, 0, 0);
	std::vector<Coordinate> neighboursPositions = start.getNeighbours(collisionGrid, zone->getResolution().x * _myCity->gridScalar, zone->getResolution().z * _myCity->gridScalar);

	int rnd = GameManager::getSingletonPtr()->getRandomInRange(0, neighboursPositions.size());
	calculateAStar(Ogre::Vector3(neighboursPositions[rnd].x, 0, neighboursPositions[rnd].z));

	_myNode->lookAt(Ogre::Vector3(nextPos[0].x, getPosition().y, nextPos[0].z), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
}

void BaseNpc::recieveItem() {
	Character::recieveItem();

	GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgNpcItem);
}

/// <returns>False if the player is too far away to start a talk</returns>
bool BaseNpc::talk()
{
	// start the dialog if it wasn't started already
	if (!_inDialog) {
		// currently in dialog
		_inDialog = true;

	}
	else {
		_dialogCount++;
		if (_dialogCount < _dialog.size()) {
			GameManager::getSingletonPtr()->getUIManager()->appendDialogText(_dialog[_dialogCount]);
		}
		else {
			if (_hasItem && _needToGiveItem) {
				giveItem(GameManager::getSingletonPtr()->getPlayer());
			}
			else if (_relevantForAction) {
				GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgPlayerInfo);
			}

			GameManager::getSingletonPtr()->getUIManager()->hideDialog();
			_dialogCount = 0;
			_inDialog = false;
		}
	}

	return _inDialog;
}
