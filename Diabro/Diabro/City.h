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

///different types of rooms
enum RoomType
{
	EmptyRT,
	CityRT,
	HideoutRT,
	HubRT,
	sizeRT
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