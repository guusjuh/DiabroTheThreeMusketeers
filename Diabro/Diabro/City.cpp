#include "City.h"
#include <stdlib.h>
#include "GameManager.h"
#include "math.h"

int City::gridScalar = 2;

/// <summary>
/// Initializes a new instance of the <see cref="City"/> class.
/// </summary>
/// <param name="pX">The x position.</param>
/// <param name="pZ">The z position.</param>
/// <param name="pWidth">Width of the city.</param>
/// <param name="pDepth">Depth of the city.</param>
/// <param name="pId">The id.</param>
/// <param name="pScalar">The scalar for the city.</param>
City::City(int pX, int pZ, int pWidth, int pDepth, int pId, int pScalar) :
position(Coordinate(pX, pZ)), width(pWidth), depth(pDepth), id(pId), scalar(pScalar)
{
	_tiles = new bool[scaledWidth() * scaledDepth()];
	for (int x = 0; x < scaledWidth(); x++) {
		for (int z = 0; z < scaledDepth(); z++) {
			setTile(x, z, false);
		}
	}
}

bool City::getTile(Coordinate pos) {
	if (pos.x < 0 || pos.z < 0 || pos.x > width || pos.z > depth) {
		return false;
	}
	return _tiles[pos.x + pos.z * scaledWidth()];
}

bool City::getTile(int x, int z) {
	if (x < 0 || z < 0 || x > width || z > depth) {
		return false;
	}
	return _tiles[x + z * scaledWidth()];
}

void City::setTile(Coordinate pos, bool value) {
	if (pos.x < 0 || pos.z < 0 || pos.x > width || pos.z > depth) {
		//Debug("Coordinate out of range", pos)
	} else {
		_tiles[pos.x + pos.z * scaledWidth()] = value;
	}
}

void City::setTile(int x, int z, bool value) {
	if (x < 0 || z < 0 || x > width || z > depth) {
		//Debug("position out of range", Coordinate(pos.x, pos.y))
	} else {
		_tiles[x + z * scaledWidth()] = value;
	}
}


/// <summary>
/// Initializes this instance.
/// </summary>
void City::init()
{
	generateBuildings();
}

/// <summary>
/// Finalizes an instance of the <see cref="City"/> class.
/// </summary>
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
void City::setType(int type)
{
	if(type == -1) {
		srand(355 * id);
		typeFlag = (RoomType)(rand() % (int)(sizeRT));
	} else {
		typeFlag = type < (int)sizeRT ? (RoomType)type : (RoomType)0;
	}
}

/// <summary>
/// Generates the buildings for the city.
/// </summary>
void City::generateBuildings()
{
	std::vector<Ogre::Entity*> buildingEntities;
	std::vector<Coordinate> buildingLocations = getBuildingPositions(1, 1);

	int buildingAmount = rand() %(width * depth - width) + width;
	//Get al possible positions

	for (int n = 0; n < buildingAmount; n++) {
		//TODO: make it an ID
		Ogre::SceneNode* buildingNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		_buildingNodes.push_back(buildingNode);

		Ogre::Entity* buildingEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("cube.mesh");
		buildingNode->setScale(1, 3, 1);
		buildingNode->attachObject(buildingEntity);
		
		//calculate building positions
		//TODO: Change the numbers here to match those provided by levelgen CHECK
		buildingNode->setPosition(0, 100, 0);

		BuildingType buildingType = (BuildingType)(typeFlag == HideoutRT ? HideOutHouse : GameManager::getSingletonPtr()->getRandomInRange(0, AMOUNT_OF_BUILDINGTYPES - 1));
		int residents = rand() % 4;
		//TODO: generate cities
		Building thisBuilding = Building(buildingType, residents, Ogre::Vector2(0, 0));
		//TODO: fill _buildings
		_buildings.push_back(thisBuilding);

		nodeList(buildingNode);
		buildingEntities.push_back(buildingEntity);		
	}

	//assign material, based on buildingtype
	assignBuildingRole(_buildings, buildingEntities);
}

std::vector<Coordinate> City::getBuildingPositions(int width, int height) {
	std::vector<Coordinate> buildingLocations;
	for (int x = 1; x < scaledWidth() - 1; x++) {
		for (int z = 1; z < scaledDepth() - 1; z++) {
			if(x == 1) {
				//left layer
				//TODO: find connections
				//1: scale current position to world coordinates
				//2: check if distance to connection is < 1
			} else if (x == scaledWidth() - 1) {
				//right layer
				//TODO: find connections
				//1: scale current position to world coordinates
				//2: check if distance to connection is < 1
			}
			if (z == 1) {
				//upper layer
				//TODO: find connections
				//1: scale current position to world coordinates
				//2: check if distance to connection is < 1
			} else if (z == scaledWidth() - 1) {
				//lower layer
				//TODO: find connections
				//1: scale current position to world coordinates
				//2: check if distance to connection is < 1
			}
		}
	}
	return buildingLocations;
}


/// <summary>
/// Assigns the building role.
/// </summary>
/// <param name="buildings">The buildings.</param>
/// <param name="pEntities">The p entities.</param>
/// <returns></returns>
void City::assignBuildingRole(std::vector<Building>  buildings, std::vector<Ogre::Entity*> pEntities)
{
	std::stringstream nodename("buildingRoleNode");
	
	for (int i = 0; i < buildings.size(); i++) {
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
}

/// <summary>
/// Nodes the list.
/// </summary>
/// <param name="pBuildingNode">The p building node.</param>
/// <returns></returns>
std::vector<Ogre::SceneNode*> City::nodeList(Ogre::SceneNode* pBuildingNode)
{
	_buildingNodes.push_back(pBuildingNode);
	return _buildingNodes;
}

/**std::vector<Ogre::SceneNode*> City::nodeIteration(Ogre::SceneNode *pNodeName)
{
	Ogre::SceneNode::ChildNodeIterator cNode = pNodeName->getChildIterator();
	while (cNode.hasMoreElements()) {
		buildings.push_back((Ogre::SceneNode *)cNode.getNext());
	}
	return buildings;
}**/

/// <summary>
/// Gets the scaled width.
/// </summary>
/// <param name="width">The width.</param>
/// <param name="scalar">The scalar.</param>
/// <returns></returns>
int City::getScaledWidth(int width, int scalar)
{
	return width*scalar;
}

/// <summary>
/// Gets the scaled depth.
/// </summary>
/// <param name="depth">The depth.</param>
/// <param name="scalar">The scalar.</param>
/// <returns></returns>
int City::getScaledDepth(int depth, int scalar)
{
	return depth*scalar;
}

/// <summary>
/// Gets the random point in room.
/// </summary>
/// <returns></returns>
Ogre::Vector3 City::getRandomPointInRoom() {
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

/// <summary>
/// Returns the positions of the buildings.
/// </summary>
/// <returns></returns>
std::vector<Coordinate> City::buildingPositions() {
	std::vector<Coordinate> positions;
	positions.clear();
	for (int i = 0; i < _buildingNodes.size(); i++) {
		Ogre::Vector3 buildingPos = _buildingNodes[i]->getPosition();
		positions.push_back(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getGridPosition(Coordinate(buildingPos.x, buildingPos.z)));
	}

	return positions;
}