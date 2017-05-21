#ifndef LEVEL_GENERATOR_H_
#define LEVEL_GENERATOR_H_
#include <OgreSceneManager.h>
#include "Zone.h"

///directions
enum DirectionType
{
	NorthDT,
	EastDT,
	SouthDT,
	WestDT
};

class LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	void restart();

	Zone getZone(Coordinate pZoneId);
	Zone getZone(int pX, int pZ);
	
	Ogre::Vector3 getWorldPosition(Coordinate pWorldCoord);
	Coordinate getGridPosition(Coordinate pGridCoord);
	Coordinate getEmptyPosition(bool pEmptyNeighbours);
	
	float getDistToSis(Ogre::Vector3 otherPos) {
		return sqrt(abs(pow((_sisterNode->getPosition() - otherPos).x, 2)
			+ pow((_sisterNode->getPosition() - otherPos).z, 2)));
	}

	Ogre::Vector3 getStartPos() {
		return getWorldPosition(_startCity->position);
	}

	int scalar; ///< scales grid position to world positions

private:
	Zone _zone[1]; ///<holds different zones in level

	City* _startCity;
	City* _endCity;
	
	Ogre::SceneNode* _sisterNode;

	void drawDungeonFloor(Zone pZone, Ogre::ColourValue pCol);

	void createTileMesh(std::string pName, Ogre::ColourValue pCol) const;
	void donaldTrump(std::string pName, Ogre::ColourValue pCol) const;

	void determineCityTypes();
	void spawnCityContent();
	void placeEnemySpawnNode(City*, int);
	void spawnNPCs(City*, Building*);

	Ogre::SceneNode* _dungeonNode;
	std::vector<Ogre::Entity*> _content;
};

#endif