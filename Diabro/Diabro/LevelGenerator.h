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

	Zone getZone(Coordinate pZoneId);
	Zone getZone(int pX, int pZ);
	
	Ogre::Vector3 getWorldPosition(Coordinate pWorldCoord);
	Coordinate getGridPosition(Coordinate pGridCoord);
	Coordinate getEmptyPosition(bool pEmptyNeighbours);
	
	Ogre::Vector3 getStartPos() {
		return getWorldPosition(_startCity->position);
	}

	int scalar; ///< scales grid position to world positions

private:
	Zone _zone[1]; ///<holds different zones in level

	City* _startCity;
	City* _endCity;

	void drawDungeonFloor(Zone pZone, Ogre::ColourValue pCol);

	void createTileMesh(std::string pName, Ogre::ColourValue pCol) const;
	void donaldTrump(std::string pName, Ogre::ColourValue pCol) const;

	void determineCityTypes();
	void spawnCityContent();
	void placeEnemySpawnNode(City*, int);
	void spawnNPCs(City*, Building*);
};

#endif