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
	
	Coordinate getWorldPosition(Coordinate pWorldCoord);
	Coordinate getGridPosition(Coordinate pGridCoord);
	Coordinate getEmptyPosition(bool pEmptyNeighbours);
	
	int scalar; ///< scales grid position to world positions

private:
	Zone _zone[1]; ///<holds different zones in level

	void drawDungeonFloor(Zone pZone, Ogre::ColourValue pCol);

	void createTileMesh(std::string pName, Ogre::ColourValue pCol) const;
	void donaldTrump(std::string pName, Ogre::ColourValue pCol) const;
};

#endif