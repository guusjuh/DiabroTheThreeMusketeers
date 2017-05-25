#include "City.h"
#include <stdlib.h>
#include "GameManager.h"
#include "math.h"

City::City(int pX, int pZ, int pWidth, int pDepth, int pId, int pScalar) :
position(Coordinate(pX, pZ)), width(pWidth), depth(pDepth), id(pId), scalar(pScalar)
{
	setRndType();
}

void City::init()
{
	generateBuildings();
}

City::~City()
{
}

/// returns the center tile of the room
Coordinate City::getCenterTile() {
	int x = ceil(position.x + width / 2.0f);
	int z = ceil(position.z + depth / 2.0f);
	return Coordinate(x, z);
}

/// returns a random tile coordinate within the city
Coordinate City::getRandomTile() {
	//TODO: prevent collision with buildings
	int x = rand() % width;
	int z = rand() % depth;
	return Coordinate(position.x + x, position.z + z);
}

/// assigns the city a random type
void City::setRndType()
{
	//TODO:improve picking method
	srand(355 * id);
	typeFlag = static_cast<RoomType> (rand() % static_cast<int>(RoomType::sizeRT));
}

void City::generateBuildings()
{
	_rootNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode();
	manager = GameManager::getSingletonPtr()->getSceneManager();
		
	std::vector<Ogre::Entity*> buildingEntities;
	int random = GameManager::getSingletonPtr()->getRandomInRange(1, 2);
	for (int i = 0; i < random; i++)
	{
		//TODO: make it an ID
		Ogre::SceneNode* buildingNode = _rootNode->createChildSceneNode();
		Ogre::Entity* _buildingEntity = manager->createEntity("cube.mesh");
		buildingNode->setScale(1, 3, 1);
		buildingNode->attachObject(_buildingEntity);
		//TODO: Change the numbers here to match those provided by levelgen CHECK
		int xPos = GameManager::getSingletonPtr()->getRandomInRange(position.x, (width - 1)) * scalar;
		int zPos = GameManager::getSingletonPtr()->getRandomInRange(position.z, (depth - 1)) * scalar;
		buildingNode->setPosition(xPos, 100, zPos); 

		int buildingType = typeFlag == HideoutRT ? HideOutHouse : GameManager::getSingletonPtr()->getRandomInRange(0, AMOUNT_OF_BUILDINGTYPES-1);
		int residents = GameManager::getSingletonPtr()->getRandomInRange(0, 3);
		Building thisBuilding = Building((BuildingType)buildingType, residents, Ogre::Vector2(xPos, zPos));
		_buildingStructs.push_back(thisBuilding);

		nodeList(buildingNode);
		buildingEntities.push_back(_buildingEntity);
	}

	assignBuildingRole(_buildingStructs, buildingEntities);
}


int City::assignBuildingRole(std::vector<Building>  buildings, std::vector<Ogre::Entity*> pEntities)
{
	std::stringstream nodename("buildingRoleNode");
	
	for (int i = 0; i < buildings.size(); i++) {

		// nodename << childIteration << "_" << parentIteration << "_" << i;
		// roles = rand() % 6;

		switch (buildings[i].type) // assign building random professions by giving them a rolenode
		{
		case Smithery:
			pEntities[i]->setMaterialName("Houses/Yellow");
			break;
		case GuardHouse:
			pEntities[i]->setMaterialName("Houses/Pink");
			break;
		case Church:
			pEntities[i]->setMaterialName("Houses/Green");
			break;
		case PotionStore:
			pEntities[i]->setMaterialName("Houses/Purple");
			break;
		case Shop:
			pEntities[i]->setMaterialName("Houses/Blue");
			break;
		case FancyHouse:
			pEntities[i]->setMaterialName("Houses/Pink");
			break;

		case HideOutHouse:
			pEntities[i]->setMaterialName("Houses/Grey");
			break;
		default:
			break;
		}
	}

	return role;
}


std::vector<Ogre::SceneNode*> City::nodeList(Ogre::SceneNode* pBuildingNode)
{
	_buildings.push_back(pBuildingNode);
	return _buildings;
}

/**std::vector<Ogre::SceneNode*> City::nodeIteration(Ogre::SceneNode *pNodeName)
{
	Ogre::SceneNode::ChildNodeIterator cNode = pNodeName->getChildIterator();
	while (cNode.hasMoreElements()) {
		buildings.push_back((Ogre::SceneNode *)cNode.getNext());
	}
	return buildings;
}**/

int City::getScaladWidth(int width, int scalar)
{
	return width*scalar;
}

int City::getScaladDepth(int depth, int scalar)
{
	return depth*scalar;
}

Ogre::Vector3 City::getRandomPointInRoom(){
	/*
	int xPos = GameManager::getSingletonPtr()->getRandomInRange(position.x * scalar, (width) * scalar);
	int zPos = GameManager::getSingletonPtr()->getRandomInRange(position.z * scalar, (depth) * scalar);
	*/
	int xPos;
	int zPos;
	switch (GameManager::getSingletonPtr()->getRandomInRange(0, 4)){
	case 0:
		xPos = position.x;
		zPos = position.z;
		break;
	case 1:
		xPos = position.x + width - 1;
		zPos = position.z;
		break;
	case 2:
		xPos = position.x;
		zPos = position.z + depth - 1;
		break;
	case 3:
		xPos = position.x + width - 1;
		zPos = position.z + depth - 1;
		break;
	}
	return Ogre::Vector3(xPos, 0, zPos);
}

std::vector<Coordinate> City::buildingPositions(){
	std::vector<Coordinate> positions;
	positions.clear();
	for (int i = 0; i < _buildings.size(); i++) {
		Ogre::Vector3 buildingPos = _buildings[i]->getPosition();
		positions.push_back(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(buildingPos.x, buildingPos.z)));
	}

	return positions;
}