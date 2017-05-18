#include "City.h"
#include <stdlib.h>
#include "GameManager.h"
#include "math.h"

City::City(int pX, int pZ, int pWidth, int pDepth, int pId) :
position(Coordinate(pX, pZ)), width(pWidth), depth(pDepth), id(pId)
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
	int random = GameManager::getSingletonPtr()->getRandomInRange(5, 15);
	for (int i = 0; i < random; i++)
	{
		//TODO: make it an ID
		Ogre::SceneNode* buildingNode = _rootNode->createChildSceneNode();
		Ogre::Entity* _buildingEntity = manager->createEntity("cube.mesh");
		buildingNode->setScale(2, 2, 2);
		buildingNode->attachObject(_buildingEntity);
		//TODO: Change the numbers here to match those provided by levelgen CHECK
		int xPos = GameManager::getSingletonPtr()->getRandomInRange(position.x * 1000, (width - 1) * 1000);
		int zPos = GameManager::getSingletonPtr()->getRandomInRange(position.z * 1000, (depth - 1) * 1000);
		buildingNode->setPosition(xPos, 100, zPos);//GameManager::getSingletonPtr()->getRandomInRange(x * 1000, (x + width) * 1000), 50, GameManager::getSingletonPtr()->getRandomInRange(z * 1000, (z + depth) * 1000));

		nodeList(buildingNode);
		buildingEntities.push_back(_buildingEntity);
	}

	assignBuildingRole(buildings, buildingEntities);
}


int City::assignBuildingRole(std::vector<Ogre::SceneNode *>  buildings, std::vector<Ogre::Entity*> pEntities)
{
	std::stringstream nodename("buildingRoleNode");
	
	for (int i = 0; i < buildings.size(); i++) {

		//nodename << childIteration << "_" << parentIteration << "_" << i;
		int roles = rand() % 6;

		switch (roles) // assign building random professions by giving them a rolenode
		{

		case 0:
			pEntities[i]->setMaterialName("Houses/Red");
			break;
		case 1:
			pEntities[i]->setMaterialName("Houses/Yellow");
			break;
		case 2:
			pEntities[i]->setMaterialName("Houses/Pink");
			break;
		case 3:
			pEntities[i]->setMaterialName("Houses/Green");
			break;
		case 4:
			pEntities[i]->setMaterialName("Houses/Purple");
			break;
		case 5:
			pEntities[i]->setMaterialName("Houses/Blue");
			break;
		default:
			break;
		}
	}

	return role;
}


std::vector<Ogre::SceneNode*> City::nodeList(Ogre::SceneNode* pBuildingNode)
{
	buildings.push_back(pBuildingNode);
	return buildings;

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

Coordinate City::getRandomPoint(){
	int xPos = GameManager::getSingletonPtr()->getRandomInRange(position.x * 1000, (width - 1) * 1000);
	int zPos = GameManager::getSingletonPtr()->getRandomInRange(position.z * 1000, (depth - 1) * 1000);

	return Coordinate(xPos, zPos);
}