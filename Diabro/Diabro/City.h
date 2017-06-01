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
private:
	Coordinate position; ///< upper left corner position of room
	RoomType typeFlag;
	bool* _tiles;
	int width;
	int depth;
	int id; ///< unique id
	int scalar;
	std::vector<Coordinate> connections;

public:
	void init();
	
	int Width() { return width; }
	int Depth() { return depth; }
	int ID(){ return id; }
	RoomType TypeFlag(){ return typeFlag; }
	Coordinate Position() { return position; }
	void addConnection(Coordinate coord){ connections.push_back(coord);}
	int nConnections(){ return connections.size(); }
	void clearConnections();
	Coordinate getConnection(int index) { return connections[index]; }

	bool getTile(Coordinate pos);
	bool getTile(int x, int z);
	int Scalar(){ return scalar; }
	int scaledWidth() { return width * gridScalar; }
	int scaledDepth() { return depth * gridScalar; }

	//TODO: make local position
	Ogre::Vector3 getRandomPointInRoom();
	Coordinate getCenterTile();
	Coordinate getRandomTile();

	City(int pX, int pZ, int pWidth, int pDepth, int pId, int pScalar);
	~City();

	std::vector<Building> Buildings() { return _buildings; };
	std::vector<Coordinate> buildingPositions();

	float getDistTo(City* other) {
		float xDist = other->position.x - position.x;
		float yDist = other->position.z - position.z;

		return sqrt(abs(pow(xDist, 2) + pow(yDist, 2)));
	}
	void setType(int type = -1);

protected:
	//std::vector<Ogre::SceneNode*> City::nodeIteration(Ogre::SceneNode *); //simple method that will iterate through all child nodes and set them in an array to eb used. (maybe for a "BaseController)
	bool checkCollision(Ogre::SceneNode *); //Checks if buildings are colliding with one another
	bool checkEntryWay(Ogre::SceneNode *); //Checks if the buildings are blocking entryways
	void assignBuildingRole(std::vector<Building> , std::vector<Ogre::Entity*>); //Assign roles to buildings in the city
	
	std::vector<Ogre::SceneNode*> nodeList(Ogre::SceneNode* pBuildingNode);
	
private:
	static int gridScalar;
	std::string _name;

	int buildingId;
	int parentIteration;
	int childIteration;

	void generateBuildings();
	int getScaledWidth(int width, int scalar);
	int getScaledDepth(int depth, int scalar);
	void setTile(int x, int z, bool value);
	void setTile(Coordinate pos, bool value);
	std::vector<RealCoordinate> getBuildingPositions();
	void printGrid();

	std::vector <Ogre::SceneNode*> _buildingNodes;
	std::vector<Building> _buildings;
	int _numberOfBuildings;
};
#endif