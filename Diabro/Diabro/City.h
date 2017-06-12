#ifndef CITY_H_
#define CITY_H_
#include <vector>
#include <OgreSceneNode.h>
#include "IQuestContent.h"
#include "Coordinate.h"
#include "math.h"


/// <summary>
/// Enum for the different city types. 
/// </summary>
enum RoomType
{
	CityRT,
	HideoutRT,

	sizeRT
};

/// <summary>
/// Enum for the different building types. Corresponds to the NPC's professions. 
/// </summary>
enum BuildingType {
	Smithery = 0,
	GuardHouse,
	Church,
	PotionStore,
	Shop,
	FancyHouse,

	HideOutHouse,

	AMOUNT_OF_BUILDINGTYPES
};

/// <summary>
/// Struct for a building in a city.
/// </summary>
struct Building {
	Building(BuildingType pType, int pResidents, Ogre::Vector2 pPosition) 
	: type(pType), residents(pResidents), position(pPosition) {}

	BuildingType type;
	int residents;
	Ogre::Vector2 position;

	Ogre::Vector3 getPosition() {
		return Ogre::Vector3(position.x, 0, position.y);
	}
};

/// <summary>
/// The city class containing buildings and inhabitants. 
/// </summary>
/// <seealso cref="IQuestContent" />
class City : public IQuestContent
{
private:
	Coordinate position; ///< upper left corner position of room
	RoomType typeFlag;
	int* _tiles;
	int width;
	int depth;
	int id; ///< unique id
	int scalar;
	std::string color;
	std::vector<Coordinate> connections;

public:
	City() {}
	City(int pX, int pZ, int pWidth, int pDepth, int pId, int pScalar, std::string color);
	~City();

	void init();
	void update();

	int Width() { return width; }
	int Depth() { return depth; }
	int ID(){ return id; }
	bool inCity(Coordinate position);
	std::string getColor(){ return color; }
	RoomType TypeFlag(){ return typeFlag; }
	Coordinate Position() { return position; }
	void addConnection(Coordinate coord){ connections.push_back(coord);}
	int nConnections(){ return connections.size(); }
	void clearConnections();
	Coordinate getConnection(int index) { return connections[index]; }

	int getTile(Coordinate pos);
	int getTile(int x, int z);
	int Scalar(){ return scalar; }
	static int gridScalar;
	int scaledWidth() { return width * gridScalar; }
	int scaledDepth() { return depth * gridScalar; }
	void removeNpcTiles();

	//TODO: make local position
	Ogre::Vector3 getRandomPointInRoom();
	Coordinate getCenterTile();
	RealCoordinate getCenterPosition();
	Coordinate getRandomTile();
	std::vector<Coordinate> getFreePositions();
	RealCoordinate getNpcPosition();

	std::vector<Building> Buildings() { return _buildings; };
	std::vector<Coordinate> buildingPositions();

	float getDistTo(City* other) {
		float xDist = other->position.x - position.x;
		float yDist = other->position.z - position.z;

		return sqrt(abs(pow(xDist, 2) + pow(yDist, 2)));
	}

	void setType(int type = -1);

	QuestContent getType() override { return typeFlag == CityRT ? TownQC : HideOutQC; }
	std::string getName() { return name; }
	Ogre::Vector3 getQuestPosition() override;

	bool inThisCity(Ogre::Vector3 worldCoord);// { return false; }
	//bool inThisCity(Coordinate gridCoord);// { return false; }
	void printGrid();

	float getEnemyRespawnTime() {
		return _enemyRespawnTime;
	}

	float getEnemyRespawnTimeForUsage() {
		_enemyRespawnTime += 5.0f;
		return _enemyRespawnTime;
	}

protected:
	void setBuildingMaterial(std::vector<Building>, std::vector<Ogre::Entity*>);

	std::vector<Ogre::SceneNode*> nodeList(Ogre::SceneNode* pBuildingNode);
	
private:
	std::string _name;

	int buildingId;
	int parentIteration;
	int childIteration;

	void generateBuildings();
	int getScaledWidth(int width, int scalar);
	int getScaledDepth(int depth, int scalar);
	void setTile(int x, int z, int value);
	void setTile(Coordinate pos, int value);
	std::vector<RealCoordinate> getBuildingPositions();

	std::vector <Ogre::SceneNode*> _buildingNodes;
	std::vector<Building> _buildings;
	int _numberOfBuildings;

	std::string name;
	std::string getRandomName(RoomType type);

	static const std::string _hideoutNameOptions[];
	static std::vector<std::string> _usedHideoutNameOptions;
	static const std::string _townNameOptions[];
	static std::vector<std::string> _usedTownNameOptions;

	float _enemyRespawnTime;
};
#endif