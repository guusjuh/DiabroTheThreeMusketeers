#ifndef CITY_H_
#define CITY_H_
#include <vector>
#include <OgreSceneNode.h>
#include "IQuestContent.h"
#include "math.h"

/// integer coordinate representing a 2D position
struct Coordinate {
	int x;
	int z;
	Coordinate(int pX, int pZ) : x(pX), z(pZ) {	}
	Coordinate() : x(0), z(0) {	}
	~Coordinate() { }
};

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

	Ogre::Vector3 getPositionInFrontOf() {
		return Ogre::Vector3(position.x, 0, position.y);
	}
};

/// <summary>
/// The city class containing buildings and inhabitants. 
/// </summary>
/// <seealso cref="IQuestContent" />
class City : public IQuestContent
{
public:
	Coordinate position; ///< upper left corner position of room
	RoomType typeFlag;
	int width;
	int depth;
	int id; ///< unique id
	int scalar;

	void init();
	void update();

	//TODO: make local position
	Ogre::Vector3 getRandomPointInRoom();
	Coordinate getCenterTile();
	Coordinate getRandomTile();

	std::vector<Coordinate> connections;
	City(){}
	City(int pX, int pZ, int pWidth, int pDepth, int pId, int pScalar);
	~City();

	std::vector<Building> Buildings() { return _buildingStructs; };

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

protected:
	//std::vector<Ogre::SceneNode*> City::nodeIteration(Ogre::SceneNode *); //simple method that will iterate through all child nodes and set them in an array to eb used. (maybe for a "BaseController)
	bool checkCollision(Ogre::SceneNode *); //Checks if buildings are colliding with one another
	bool checkEntryWay(Ogre::SceneNode *); //Checks if the buildings are blocking entryways
	int assignBuildingRole(std::vector<Building> , std::vector<Ogre::Entity*>); //Assign roles to buildings in the city
	int getScaledWidth(int width, int scalar);
	int getScaledDepth(int depth, int scalar);
	std::vector<Ogre::SceneNode*> nodeList(Ogre::SceneNode* pBuildingNode);
	
private:
	std::string _name;

	int buildingId;
	int parentIteration;
	int childIteration;

	void generateBuildings();

	int _scaledWidth;
	int _scaledDepth;
	int *_tiles2;
	std::vector <Ogre::SceneNode*> _buildings;
	std::vector<Building> _buildingStructs;
	Ogre::SceneManager *manager;
	Ogre::Entity* _signEntity;
	Ogre::SceneNode* _rootNode;
	Ogre::SceneNode* _roleNode;
	int _numberOfBuildings;
	int role;

	std::vector<std::string> getNameOptions(RoomType type);
	std::string name;
};
#endif