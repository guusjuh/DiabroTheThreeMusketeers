#include "City.h"
#include <stdlib.h>
#include "GameManager.h"
#include "math.h"

int City::gridScalar = 2;

const std::string City::_hideoutNameOptions[19] = 
{ "Ardougne", "Askroth", "Barbarian Village", "Dis", "Dorgesh-Kaan", "Gu'Tanoth", "Jatizso", "Kanatah",
  "Menaphos", "Mort'ton", "Musa Point", "Nardah", "Neitiznot", "Onkhara", "Oo'glog", "Pollniveach",
  "Tyras Camp", "Uzer", "Zanaris" };

std::vector<std::string> City::_usedHideoutNameOptions;

const std::string City::_townNameOptions[30] = 
{ "Al Kharid", "Arposandra", "Brimhaven", "Burthorpe", "Canifis", "Catherby", "Draynor Village", "Edgeville",
  "Etceteria", "Falador", "Hemenster", "Keldagrim", "Lletya", "Lumbridge", "Marim", "Meiyerditch",
  "Mos Le'Harmless", "Otot", "Palingrad", "Prifddinas", "Rellekka", "Rimmington", "Seers' Village",
  "Shilo Village", "Sophanem", "Taverley", "Tzhaar", "Ullek", "Varrock", "Yanille" };

std::vector<std::string> City::_usedTownNameOptions;

Coordinate operator- (Coordinate &lhs, Coordinate &rhs) {
	return (Coordinate(lhs.x - rhs.x, lhs.z - rhs.z));
}

Coordinate operator* (Coordinate &lhs, int &rhs) {
	return Coordinate(lhs.x * rhs, lhs.z * rhs);
}

Coordinate operator* (Coordinate &lhs, float &rhs){
	return Coordinate(static_cast<int>(lhs.x * rhs), static_cast<int>(lhs.z * rhs));
}

void Coordinate::operator=(RealCoordinate& rhs) {
	x = static_cast<int>(rhs.rx);
	z = static_cast<int>(rhs.rz);
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
City::City(int pX, int pZ, int pWidth, int pDepth, int pId, int pScalar, std::string color) :
position(Coordinate(pX, pZ)), width(pWidth), depth(pDepth), id(pId), scalar(pScalar), color(color), _enemyRespawnTime(5.0f)
{
	_relevantForAction = false;
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
	if (pos.x < 0 || pos.z < 0 || pos.x >= scaledWidth() || pos.z >= scaledDepth() 
		|| _tiles == nullptr) {
		return false;
	}
	return _tiles[pos.x + pos.z * scaledWidth()];
}

int City::getTile(int x, int z) {
	if (x < 0 || z < 0 || x >= scaledWidth() || z >= scaledDepth() 
		|| _tiles == nullptr) {
		return -1;
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

	name = getRandomName(typeFlag);
}

/// <summary>
/// Finalizes an instance of the <see cref="City"/> class.
/// </summary>
City::~City() { }

void City::update() {
	if(_relevantForAction) {
		if(inThisCity(GameManager::getSingletonPtr()->getPlayer()->getPosition())) {
			GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest()->sendMsg(this, Action::msgCityReached);
		}
	}
}

bool City::inThisCity(Ogre::Vector3 worldCoord) {
	worldCoord += (Zone::scalar / (City::gridScalar*2));

	Coordinate coord = Coordinate(worldCoord.x / Zone::scalar, worldCoord.z / Zone::scalar);

	return inCity(coord, true);
}

std::string City::getRandomName(RoomType type) {
	std::vector<std::string> _currOptions;
	std::string _returnName;

	bool occured = false;
	int lengthNameOptions;

	if(type == CityRT) {
		lengthNameOptions = sizeof(_townNameOptions) / sizeof(_townNameOptions[0]);
		for (int i = 0; i < lengthNameOptions; ++i) {
			occured = false;

			for (int j = 0; j < _usedTownNameOptions.size(); ++j) {
				if (_townNameOptions[i] == _usedTownNameOptions[j]) {
					occured = true;
				}
			}

			if (!occured) _currOptions.push_back(_townNameOptions[i]);
		}

		// if there are no available names anymore, start reusing
		if (_currOptions.size() == 0) {
			_usedTownNameOptions.clear();

			// call this method again 
			// now that the vector is empty, names will be reused
			_returnName = getRandomName(CityRT);
		}
		else {
			_returnName = _currOptions[GameManager::getSingletonPtr()->getRandomInRange(0, _currOptions.size())];
		}

		_usedTownNameOptions.push_back(_returnName);

	} else if(type == HideoutRT) {
		lengthNameOptions = sizeof(_hideoutNameOptions) / sizeof(_hideoutNameOptions[0]);
		for (int i = 0; i < lengthNameOptions; ++i) {
			occured = false;

			for (int j = 0; j < _usedHideoutNameOptions.size(); ++j) {
				if (_hideoutNameOptions[i] == _usedHideoutNameOptions[j]) {
					occured = true;
				}
			}

			if (!occured) _currOptions.push_back(_hideoutNameOptions[i]);
		}

		// if there are no available names anymore, start reusing
		if (_currOptions.size() == 0) {
			_usedHideoutNameOptions.clear();

			// call this method again 
			// now that the vector is empty, names will be reused
			_returnName = getRandomName(HideoutRT);
		}
		else {
			_returnName = _currOptions[GameManager::getSingletonPtr()->getRandomInRange(0, _currOptions.size())];
		}

		_usedHideoutNameOptions.push_back(_returnName);

	} else {
		Debug("!\tThis is not a valid room type.");
	}

	return _returnName;
}

///returns whether a (zoneGrid) position is in the current grid
/// \param position the position to check
bool City::inCity(Coordinate position, bool outerBorders)
{
	Coordinate min = this->position;
	Coordinate max = this->position + Coordinate(width, depth);

	if (outerBorders){
		min.x--;
		min.z--;
		max.x++;
		max.z++;
	}

	if (position.x >= min.x && position.z >= min.z)
	{
		if (position.x < max.x && position.z < max.z)
		{
			return true;
		}
	}
	return false;
}

/// returns the center tile of the room
Coordinate City::getCenterTile() {
	int x = floor(position.x + width / 2.0f);	
	int z = floor(position.z + depth / 2.0f);
	return Coordinate(x, z);
}

RealCoordinate City::getCenterPosition() {
	float x = position.x + width / 2.0f;
	float z = position.z + depth / 2.0f;
	return RealCoordinate(x, z);
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
		srand(355*id);
		typeFlag = (RoomType)(rand() % (int)(sizeRT));
	} else {
		typeFlag = type < (int)sizeRT ? (RoomType)type : (RoomType)0;
	}
}

std::vector<Coordinate> City::getFreePositions() {
	if (typeFlag == CityRT) {
		Coordinate temp = getCenterTile();
		temp -= position;
		temp = temp * gridScalar;
		setTile(temp, 2);
	}
	
	std::vector<Coordinate> freePositions = std::vector<Coordinate>();
	for (int x = 0; x < scaledWidth(); x++) {
		for (int z = 0; z < scaledDepth(); z++) {
			if (!getTile(x,z)) {
				freePositions.push_back(Coordinate(x, z));
			}
		}
	}

	//printGrid();
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
		//printGrid();

		RealCoordinate returnPos = RealCoordinate(position.x, position.z);
		if (positions[rnd].x != 0) returnPos.rx += static_cast<float>(positions[rnd].x) / static_cast<float>(gridScalar);
		if (positions[rnd].z != 0) returnPos.rz += static_cast<float>(positions[rnd].z) / static_cast<float>(gridScalar);
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

	// get all possible positions (for cities, not hideouts)
	std::vector<RealCoordinate> buildingLocations;

	//y position of all buildings in the city
	int y;
	RealCoordinate offset = RealCoordinate(0, 0);
	Ogre::Vector3 scale = Ogre::Vector3(0, 0, 0);
	int buildingAmount = 0; 
	if (typeFlag == HideoutRT) {
		buildingAmount = 1;

		y = 250;
		scale = Ogre::Vector3(5.0f, 5.0f, 5.0f);
	}
	else {// ==  cityRT
		buildingLocations = getBuildingPositions();
		if (buildingLocations.size() < 1) return;

		buildingAmount = rand() % ((width > depth) ? width : depth) + ((width < depth) ? width : depth) / 2;
		if (buildingAmount <= 1) buildingAmount = 2;

		y = 100;
		scale = Ogre::Vector3(2.5f, 3.0f, 2.5f);
	}
	
	// for each building to be generated
	for (int n = 0; n < buildingAmount; n++) {
		RealCoordinate pos;
		
		// if no locations, continue
		int rnd = 0;
		
		if (buildingLocations.size() == 1) rnd = 0;
		else if (buildingLocations.size() > 1) rnd = rand() % buildingLocations.size();
		
		int residents = rand() % 2 + 1;;
		BuildingType buildingType;

		// spawn enemy building
		if (typeFlag == HideoutRT){
			pos = getCenterTile();
			pos = pos * scalar;
			pos.rx += 125;
			pos.rz += 125;
			Coordinate enemyCoord = getCenterTile();
			enemyCoord -= position;
			
			for (int x = 0; x < 2; x++) {
				for (int z = 0; z < 2; z++) {
					setTile((enemyCoord * gridScalar) + Coordinate(x, z), 1);
				}
			}
			
			residents += 2;
			
			buildingType = HideOutHouse;
		} else { // spawn npc building
			//translate object half a tile in the positive direction because the pivot of the object is at center
			// scalar (1 zone unit in world size) / GridScalar (now one city tile) / 2 (half a city tile as buildings have their pivot centered)
			pos = buildingLocations[rnd];
			RealCoordinate rc = RealCoordinate((pos.rx / scalar), (pos.rz / scalar));
			rc -= position;
			rc = rc * gridScalar;
			Coordinate friendlyCoord = Coordinate(floor(rc.rx), floor(rc.rz));

			while(getTile(friendlyCoord)) {
				if(buildingLocations.size() <= 0) {
					Debug("No building positions available.");
					//assign roles for all other buildings in city
					setBuildingMaterial(_buildings, buildingEntities);
					return;
				}
				rnd = rand() % buildingLocations.size();
				pos = buildingLocations[rnd];
				RealCoordinate rc = RealCoordinate((pos.rx / scalar), (pos.rz / scalar));
				buildingLocations.erase(buildingLocations.begin() + rnd);
				rc -= position;
				rc = rc * gridScalar;
				friendlyCoord = Coordinate(floor(rc.rx), floor(rc.rz));
			}
			setTile(friendlyCoord, 1);

			buildingType = (BuildingType)GameManager::getSingletonPtr()->getRandomInRange(0, AMOUNT_OF_BUILDINGTYPES - 1);
		}

		// create the node + entity for the building
		Ogre::SceneNode* buildingNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		_buildingNodes.push_back(buildingNode);
		Ogre::Entity* buildingEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("cube.mesh");
		buildingNode->setScale(scale);
		buildingNode->attachObject(buildingEntity);

		buildingNode->setPosition(pos.rx, y, pos.rz);

		//TODO: generate cities
		Building thisBuilding = Building(buildingType, residents, Ogre::Vector2((pos.rx), (pos.rz)));
		//TODO: fill _buildings
		_buildings.push_back(thisBuilding);

		nodeList(buildingNode);
		buildingEntities.push_back(buildingEntity);
	}
	//assign material, based on buildingtype
	setBuildingMaterial(_buildings, buildingEntities);
}

void City::setBuildingMaterial(std::vector<Building> buildings, std::vector<Ogre::Entity*> entities)
{
	for (int i = 0; i < buildings.size(); i++)
	{
		if (typeFlag == HideoutRT)
		{
			entities[i]->setMaterialName("InGame/GreyHideoutHouse");
		} else
		{
			entities[i]->setMaterialName("InGame/" + color + "House");
		}
		
	}
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
/*#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
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
#endif*/
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
		xPos = position.x * City::gridScalar;
		zPos = position.z * City::gridScalar;
		break;
	case 1:
		xPos = position.x * City::gridScalar + width * City::gridScalar - 1;
		zPos = position.z * City::gridScalar;
		break;
	case 2:
		xPos = position.x * City::gridScalar;
		zPos = position.z * City::gridScalar + depth * City::gridScalar - 1;
		break;
	case 3:
		xPos = position.x * City::gridScalar + width * City::gridScalar - 1;
		zPos = position.z * City::gridScalar + depth * City::gridScalar - 1;
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
		positions.push_back(GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getCollisionGridPosition(Coordinate(buildingPos.x, buildingPos.z)));
	}

	return positions;
}

Ogre::Vector3 City::getQuestPosition() {
	return GameManager::getSingletonPtr()->getLevelManager()->getLevelGenerator()->getWorldPosition(getCenterTile());
}
