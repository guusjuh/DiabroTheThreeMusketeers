#ifndef CITY_H_
#define CITY_H_
#include <vector>
#include <OgreSceneNode.h>

/// integer coordinate representing a 2D position
struct Coordinate {
	int x;
	int z;
	Coordinate(int pX, int pZ) : x(pX), z(pZ) {	}
	Coordinate() : x(0), z(0) {	}
	~Coordinate() { }
};

// Rosa: commented out empty and hub since i didn't see good use for it and i needed the cities and hideouts
enum RoomType
{
	//EmptyRT,
	CityRT,
	HideoutRT,
	//HubRT,
	sizeRT
};

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

class City
{
public:
	Coordinate position; ///< upper left corner position of room
	RoomType typeFlag;
	int width;
	int depth;
	int id; ///< unique id
	int scalar;
	void init();
	City(int pX, int pZ, int pWidth, int pHeight, int pId, int scalar);
	
	//TODO: make local position
	Coordinate getRandomPoint();

	std::vector<Coordinate> connections;
	City(int pX, int pZ, int pWidth, int pDepth, int pId);
	~City();

	Coordinate getCenterTile();
	Coordinate getRandomTile();
	std::vector<Coordinate> buildingPositions();

private:
	void setRndType();


protected:
	//std::vector<Ogre::SceneNode*> City::nodeIteration(Ogre::SceneNode *); //simple method that will iterate through all child nodes and set them in an array to eb used. (maybe for a "BaseController)
	bool checkCollision(Ogre::SceneNode *); //Checks if buildings are colliding with one another
	bool checkEntryWay(Ogre::SceneNode *); //Checks if the buildings are blocking entryways
	int assignBuildingRole(std::vector<Ogre::SceneNode*> , std::vector<Ogre::Entity*>); //Assign roles to buildings in the city
	int getScaladWidth(int width, int scalar);
	int getScaladDepth(int depth, int scalar);
	std::vector<Ogre::SceneNode*> nodeList(Ogre::SceneNode* pBuildingNode);
	
private:
	
	int buildingId;
	int parentIteration;
	int childIteration;
	void generateBuildings();
	int _scaledWidth;
	int _scaledDepth;
	int *_tiles2;
	std::vector <Ogre::SceneNode*> buildings;
	Ogre::SceneManager *manager;
	Ogre::Entity* _signEntity;
	Ogre::SceneNode* _rootNode;
	Ogre::SceneNode* _roleNode;
	int _numberOfBuildings;
	int role;
};
#endif