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
	//wander();
}

/// <summary>
/// Updates the frame based on the specified pDeltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void BaseNpc::update(Ogre::Real pDeltatime)
{
	Character::update(pDeltatime);

	detectPlayer();

	if (!_playerDetected) {
		_timer += pDeltatime;

		if (_timer > 2)
		{
			_timer = 0;
			wander();
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
/// Lets this instance wander randomly.
/// </summary>
void BaseNpc::wander() 
{
	Coordinate coord = _myCity->getRandomPoint();
	walkTo(Ogre::Vector3(coord.x, getPosition().y, coord.z));
}

void BaseNpc::walkTo(Ogre::Vector3 targetPos) 
{
	_myNode->lookAt(Ogre::Vector3(targetPos.x, getPosition().y, targetPos.z), Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_X);
	_dirVec = Ogre::Vector3(1, 0, 0);
}

void BaseNpc::calculateAStar(Ogre::Vector3 targetPos){

	Zone zone = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0);
	bool* collisionGrid = zone.getCollisionGrid();
	bool* grid = collisionGrid;
	int scale = _myCity->scalar;
	Ogre::Vector2 targetPosition = Ogre::Vector2(targetPos.x / scale, targetPos.z / scale);

	if (!collisionGrid[(int)(targetPosition.x + (targetPosition.y * zone._width))]){
		return;
	}

	std::vector<Node> openList;
	std::vector<Node> closedList;
	
	Ogre::Vector3 currentPos = getPosition();

	Node start = Node(currentPos.x / scale, currentPos.z / scale, targetPosition.x, targetPosition.y);
	openList.push_back(start);
	collisionGrid[start.x + start.y * zone._width] = false;

	bool targetAdded = false;
	while (!targetAdded){
		int lowestF = 1000000000;
		Node* lowestFNode;
		int pos = 0;
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i].f() < lowestF){
				lowestF = openList[i].f();
				lowestFNode = &openList[i];
				pos = i;
			}
		}
		closedList.push_back(*lowestFNode);
		if (lowestFNode->x == targetPosition.x && lowestFNode->y == targetPosition.y){
			targetAdded = true;
		}
		
		//remove the closed node from the openlist
		openList.erase(openList.begin() + pos);

		if (lowestFNode->x - 1 >= 0){
			if (collisionGrid[lowestFNode->x - 1 + (lowestFNode->y) * zone._width]){
				collisionGrid[lowestFNode->x - 1 + (lowestFNode->y) * zone._width] = false;
				openList.push_back(Node(lowestFNode, lowestFNode->x - 1, lowestFNode->y + 0, targetPosition.x, targetPosition.y));
			}
			else
			{
				for (size_t i = 0; i < openList.size(); i++)
				{
					if (openList[i].x == lowestFNode->x - 1 && openList[i].y == lowestFNode->y){
						openList[i].changeParent(lowestFNode);//checks for more efficient parent
					}
				}
			}
		}
		if (lowestFNode->x + 1 <= zone._width){
			if (collisionGrid[lowestFNode->x + 1 + (lowestFNode->y) * zone._width]){
				collisionGrid[lowestFNode->x + 1 + (lowestFNode->y) * zone._width] = false;
				openList.push_back(Node(lowestFNode, lowestFNode->x - 1, lowestFNode->y + 0, targetPosition.x, targetPosition.y));
			}
			else
			{
				for (size_t i = 0; i < openList.size(); i++)
				{
					if (openList[i].x == lowestFNode->x + 1 && openList[i].y == lowestFNode->y){
						openList[i].changeParent(lowestFNode);//checks for more efficient parent
					}
				}
			}
		}
		if (lowestFNode->y - 1 >= 0){
			if (collisionGrid[lowestFNode->x + (lowestFNode->y - 1) * zone._width]){
				collisionGrid[lowestFNode->x  + (lowestFNode->y - 1) * zone._width] = false;
				openList.push_back(Node(lowestFNode, lowestFNode->x - 1, lowestFNode->y + 0, targetPosition.x, targetPosition.y));
			}
			else
			{
				for (size_t i = 0; i < openList.size(); i++)
				{
					if (openList[i].x == lowestFNode->x && openList[i].y == lowestFNode->y - 1){
						openList[i].changeParent(lowestFNode);//checks for more efficient parent
					}
				}
			}
		}
		if (lowestFNode->y + 1 <= zone._depth){
			if (collisionGrid[lowestFNode->x + (lowestFNode->y + 1) * zone._width]){
				collisionGrid[lowestFNode->x + (lowestFNode->y + 1) * zone._width] = false;
				openList.push_back(Node(lowestFNode, lowestFNode->x - 1, lowestFNode->y + 0, targetPosition.x, targetPosition.y));
			}
			else
			{
				for (size_t i = 0; i < openList.size(); i++)
				{
					if (openList[i].x == lowestFNode->x && openList[i].y == lowestFNode->y + 1){
						openList[i].changeParent(lowestFNode);//checks for more efficient parent
					}
				}
			}
		}
		/*
		//diagonal
		if (lowestFNode->x - 1 >= 0 && lowestFNode->y - 1 >= 0){
			if (collisionGrid[lowestFNode->x - 1 + (lowestFNode->y - 1) * zone._width])
			{

			}
			else
			{
				for (size_t i = 0; i < openList.size(); i++)
				{
					if (openList[i].x == lowestFNode->x + i && openList[i].y == lowestFNode->y){
						openList[i].changeParent(lowestFNode);//checks for more efficient parent
					}
				}
			}
		}*/
		//check if openlist is empty
		if (openList.size() == 0){
			//break out of the loop target can't be found
			return;
		}
	}
}