#include "City.h"
#include <stdlib.h>
#include "GameManager.h"
#include "math.h"

int City::gridScalar = 2;

Coordinate operator+ (Coordinate &lhs, Coordinate &rhs) {
	return (Coordinate(lhs.x + rhs.x, lhs.z + rhs.z));
}

Coordinate operator- (Coordinate &lhs, Coordinate &rhs) {
	return (Coordinate(lhs.x - rhs.x, lhs.z - rhs.z));
}

Coordinate operator* (Coordinate &lhs, int &rhs) {
	return Coordinate(lhs.x * rhs, lhs.z * rhs);
}

Coordinate operator/ (Coordinate &lhs, int &rhs) {
	if(rhs == 0) {
		return Coordinate();
	}
	return Coordinate((lhs.x <= 0) ? 0 : lhs.x / rhs, (lhs.z <= 0) ? 0 : lhs.z / rhs);
}

bool operator== (Coordinate &lhs, Coordinate &rhs) {
	return (lhs.x == rhs.x && lhs.z == rhs.z);
}

RealCoordinate operator/ (RealCoordinate &lhs, float &rhs) {
	if (rhs == 0) {
		return RealCoordinate();
	}
	return RealCoordinate((lhs.rx <= 0) ? 0 : lhs.rx / rhs, (lhs.rz <= 0) ? 0 : lhs.rz / rhs);
}

RealCoordinate operator* (RealCoordinate &lhs, float &rhs) {
	return RealCoordinate(lhs.rx * rhs, lhs.rz * rhs);
}

RealCoordinate operator* (RealCoordinate &lhs, int &rhs) {
	return RealCoordinate(lhs.rx * rhs, lhs.rz * rhs);
}

RealCoordinate operator- (RealCoordinate &lhs, RealCoordinate &rhs) {
	return RealCoordinate(lhs.rx - rhs.rx, lhs.rz - rhs.rz);
}

RealCoordinate operator+ (RealCoordinate &lhs, RealCoordinate &rhs) {
	return RealCoordinate(lhs.rx + rhs.rx, lhs.rz + rhs.rz);
}

RealCoordinate operator+ (RealCoordinate &lhs, Coordinate &rhs) {
	return RealCoordinate(lhs.rx + rhs.x, lhs.rz + rhs.z);
}
RealCoordinate operator- (RealCoordinate &lhs, Coordinate &rhs) {
	return RealCoordinate(lhs.rx - rhs.x, lhs.rz - rhs.z);
}

void RealCoordinate::operator+= (Coordinate &rhs) {
	rx += rhs.x;
	rz += rhs.z;
}

void RealCoordinate::operator+= (RealCoordinate &rhs) {
	rx += rhs.rx;
	rz += rhs.rz;
}

RealCoordinate& RealCoordinate::operator= (const Coordinate &value) {
	return RealCoordinate(value.x, value.z);
}

bool operator== (RealCoordinate &lhs, RealCoordinate &rhs) {
	return (lhs.rx == rhs.rx && lhs.rz == rhs.rz);
}

bool operator== (RealCoordinate &lhs, Coordinate &rhs) {
	return (lhs.rx - rhs.x < 0.01f && lhs.rz - rhs.z < 0.01f);
}

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
	std::vector<RealCoordinate> buildingLocations = getBuildingPositions();

	int buildingAmount = rand() %(width * depth - width) + width;
	//Get al possible positions

	for (int n = 0; n < buildingLocations.size(); n++) {
		//TODO: make it an ID
		Ogre::SceneNode* buildingNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		_buildingNodes.push_back(buildingNode);

		Ogre::Entity* buildingEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("cube.mesh");
		buildingNode->setScale(1, 3, 1);
		buildingNode->attachObject(buildingEntity);
		
		//calculate building positions
		//TODO: Change the numbers here to match those provided by levelgen CHECK
		RealCoordinate transl = RealCoordinate((buildingNode->getScale().x / gridScalar) * -scalar, (buildingNode->getScale().z / gridScalar) * -scalar);
		RealCoordinate pos = transl + buildingLocations[n];
		buildingNode->setPosition(pos.rx, 100, pos.rz);

		BuildingType buildingType = (BuildingType)(typeFlag == HideoutRT ? HideOutHouse : GameManager::getSingletonPtr()->getRandomInRange(0, AMOUNT_OF_BUILDINGTYPES - 1));
		int residents = rand() % 4;
		//TODO: generate cities
		Building thisBuilding = Building(buildingType, residents, Ogre::Vector2((buildingLocations[n].rx), (buildingLocations[n].rz)));
		//TODO: fill _buildings
		_buildings.push_back(thisBuilding);

		nodeList(buildingNode);
		buildingEntities.push_back(buildingEntity);		
	}

	//assign material, based on buildingtype
	assignBuildingRole(_buildings, buildingEntities);
}

std::vector<RealCoordinate> City::getBuildingPositions() {
	std::vector<RealCoordinate> buildingLocations;
	for (int x = 1; x < scaledWidth(); x++) {
		for (int z = 1; z < scaledDepth(); z++) {
			Coordinate upperPos; // zone grid
			Coordinate lowerPos = Coordinate(x,z); // city grid
			RealCoordinate worldPos; // world positions

			worldPos = RealCoordinate(static_cast<float>(x) / static_cast<float>(gridScalar), static_cast<float>(z) / static_cast<float>(gridScalar));
			worldPos += position;
			upperPos = Coordinate(static_cast<int>(worldPos.rx), static_cast<int>(worldPos.rz));
			worldPos = worldPos * scalar;

			Coordinate translation = Coordinate(0, 0);

			if (x == 1) {
				translation.x = -1;
			} else if (x == scaledWidth() - 1) {
				translation.x = 1;
			}

			if (z == 1) {
				translation.z = -1;
			} else if (z == scaledDepth() - 1){
				translation.z = 1;
			}
			
			if (translation.x == 0 && translation.z == 0) {
				
			} else {
				std::vector<Coordinate> temp;
				if (translation.x != 0) {
					temp.push_back(Coordinate(upperPos.x + translation.x, upperPos.z));
				}
				if (translation.z != 0) {
					temp.push_back(Coordinate(upperPos.x, upperPos.z + translation.z));
				}
				bool add = true;
				for (int i = 0; i < connections.size(); i++) {
					for (int j = 0; j < temp.size(); j++) {
						if (temp[j] == connections[i]) {
							add = false;
						}
					}
				}
				if (add) {
					buildingLocations.push_back(worldPos);
				}
				else {
					int i = 0;
				}
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

void City::clearConnections()
{
	connections.clear();
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