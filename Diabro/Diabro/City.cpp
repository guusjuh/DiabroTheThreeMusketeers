#include "City.h"
#include <stdlib.h>
#include "GameManager.h"
#include "math.h"

int City::gridScalar = 2;

Coordinate operator- (Coordinate &lhs, Coordinate &rhs) {
	return (Coordinate(lhs.x - rhs.x, lhs.z - rhs.z));
}

Coordinate operator* (Coordinate &lhs, int &rhs) {
	return Coordinate(lhs.x * rhs, lhs.z * rhs);
}

Coordinate operator/ (Coordinate &lhs, int &rhs) {
	if (rhs == 0) {
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

void Coordinate::operator+=(Coordinate& rhs) {
	x += rhs.x;
	z += rhs.x;
}

void Coordinate::operator-=(Coordinate& rhs){
	x -= rhs.x;
	z -= rhs.z;
}


void RealCoordinate::operator+= (Coordinate &rhs) {
	rx += rhs.x;
	rz += rhs.z;
}

void RealCoordinate::operator+= (RealCoordinate &rhs) {
	rx += rhs.rx;
	rz += rhs.rz;
}

void RealCoordinate::operator= (const Coordinate &value) {
	rx = static_cast<float>(value.x);
	rz = static_cast<float>(value.z);
}

void RealCoordinate::operator-= (RealCoordinate &rhs){
	rx -= rhs.rx;
	rz -= rhs.rz;
}

void RealCoordinate::operator-=(Coordinate& rhs) {
	rx -= rhs.x;
	rz -= rhs.z;
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
	_tiles = new int[scaledWidth() * scaledDepth()];
	int mx = scaledWidth();
	int mz = scaledDepth();
	for (int x = 0; x < scaledWidth(); x++) {
		for (int z = 0; z < scaledDepth(); z++) {
			setTile(x, z, 0);
		}
	}
}

int City::getTile(Coordinate pos) {
	if (pos.x < 0 || pos.z < 0 || pos.x >= scaledWidth() || pos.z >= scaledDepth()) {
		return false;
	}
	return _tiles[pos.x + pos.z * scaledWidth()];
}

int City::getTile(int x, int z) {
	if (x < 0 || z < 0 || x >= scaledWidth() || z >= scaledDepth()) {
		return false;
	}
	return _tiles[x + z * scaledWidth()];
}

void City::setTile(Coordinate pos, int value) {
	if (pos.x < 0 || pos.z < 0 || pos.x >= scaledWidth() || pos.z >= scaledDepth()) {
		//Debug("Coordinate out of range", pos)
	} else {
		_tiles[pos.x + pos.z * scaledWidth()] = value;
	}
}

void City::setTile(int x, int z, int value) {
	if (x < 0 || z < 0 || x >= scaledWidth() || z >= scaledDepth()) {
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
	int x = floor(position.x + width / 2.0f);
	int z = floor(position.z + depth / 2.0f);
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

std::vector<Coordinate> City::getFreePositions() {
	std::vector<Coordinate> freePositions = std::vector<Coordinate>();
	for (int x = 0; x < scaledWidth(); x++) {
		for (int z = 0; z < scaledDepth(); z++) {
			if (!getTile(x,z)) {
				freePositions.push_back(Coordinate(x, z));
			}
		}
	}
	return freePositions;
}

void City::removeNpcTiles() {
	for (int x = 0; x < scaledWidth(); x++) {
		for (int z = 0; z < scaledDepth(); z++) {
			if (getTile(x, z) == 2) {
				setTile(x, z, 0);
			}
		}
	}
}


RealCoordinate City::getNpcPosition() {
	std::vector<Coordinate> positions = getFreePositions();
	if (positions.size() > 0) {
		int rnd = rand() % positions.size();
		setTile(positions[rnd], 2);
		RealCoordinate returnPos = RealCoordinate(position.x - 0.25f, position.z - 0.25f);
		if (positions[rnd].x != 0) returnPos.rx += static_cast<float>(positions[rnd].x) / gridScalar;
		if (positions[rnd].z != 0) returnPos.rz += static_cast<float>(positions[rnd].z) / gridScalar;
		returnPos = returnPos * scalar;
		return returnPos;
	} else {
		return RealCoordinate(-1, -1);
	}
}

/// <summary>
/// Generates the buildings for the city.
/// </summary>
void City::generateBuildings()
{
	std::vector<Ogre::Entity*> buildingEntities;
	std::vector<RealCoordinate> buildingLocations = getBuildingPositions();

	int buildingAmount = rand() % (width * depth - width) + width - 2;
	if (typeFlag == HideoutRT) buildingAmount = 1;
	//Get al possible positions

	for (int n = 0; n < buildingAmount; n++) {
	//for (int n = 0; n < buildingLocations.size(); n++) {
		if (buildingLocations.size() < 1) continue;
		//TODO: make it an ID
		Ogre::SceneNode* buildingNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		_buildingNodes.push_back(buildingNode);

		Ogre::Entity* buildingEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("cube.mesh");
		if (typeFlag == CityRT) buildingNode->setScale(1, 3, 1);
		else buildingNode->setScale(2.0f, 5, 2.0f);
		buildingNode->attachObject(buildingEntity);

		//calculate building positions
		//TODO: Change the numbers here to match those provided by levelgen CHECK
		int rnd = 0;
		rnd = rand() % buildingLocations.size();
		
		RealCoordinate pos;

		if (typeFlag == HideoutRT){
			pos = getCenterTile();
			pos = pos * scalar;
			
			Coordinate enemyCoord = getCenterTile();
			enemyCoord -= position;
			
			for (int x = 0; x < 2; x++) {
				for (int z = 0; z < 2; z++) {
					setTile(enemyCoord + Coordinate(x, z), 1);
				}
			}
		}
		else {
			//translate object half a tile in the positive direction because the pivot of the object is at center
			// scalar (1 zone unit in world size) / GridScalar (now one city tile) / 2 (half a city tile as buildings have their pivot centered)
			RealCoordinate transl = RealCoordinate(-125.0f, -125.0f);
			pos = buildingLocations[n];
			//buildingLocations.erase(buildingLocations.begin() + rnd);
			RealCoordinate rc = RealCoordinate((pos.rx / scalar), (pos.rz / scalar));
			rc -= position;
			rc = rc * gridScalar;
			Coordinate friendlyCoord = Coordinate(floor(rc.rx), floor(rc.rz));
			if (getTile(friendlyCoord)) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				FILE* fp;
				freopen_s(&fp, "CONOUT$", "w", stdout);
				printf("check city.cpp 292 jasper \n");
				fclose(fp);
#endif
			}
			while(getTile(friendlyCoord)) {
				rnd = rand() % buildingLocations.size();
				RealCoordinate rc = RealCoordinate((buildingLocations[rnd].rx / scalar), (buildingLocations[rnd].rz / scalar));
				buildingLocations.erase(buildingLocations.begin() + rnd);
				rc -= position;
				rc = rc * gridScalar;
				friendlyCoord = Coordinate(floor(rc.rx), floor(rc.rz));
			}
			pos += transl;
			setTile(friendlyCoord, 1);
			printGrid();
			int i = 0;
		}

		buildingNode->setPosition(pos.rx, 100, pos.rz);

		BuildingType buildingType = (BuildingType)GameManager::getSingletonPtr()->getRandomInRange(0, AMOUNT_OF_BUILDINGTYPES - 1);
		int residents = rand() % 3 + 1;
		if (typeFlag == HideoutRT)
		{
			residents += 2;
			buildingType = HideOutHouse;
		}
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
	for (int x = 1; x < scaledWidth() - 1; x++) {
		for (int z = 1; z < scaledDepth() - 1; z++) {
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
			} else if (x == scaledWidth() - 2) {
				translation.x = 1;
			}

			if (z == 1) {
				translation.z = -1;
			} else if (z == scaledDepth() - 2){
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

void City::printGrid() {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	for (int ix = 0; ix < scaledWidth(); ++ix) {
		for (int iz = 0; iz < scaledDepth(); ++iz) {
			printf("|%s|", getTile(ix, iz)?".":"0");
		}
		printf("\n");
	}
	printf("\n");
	fclose(fp);
	//printValues();
#endif
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