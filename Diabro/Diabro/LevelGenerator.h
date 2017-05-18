#ifndef LEVEL_GENERATOR_H_
#define LEVEL_GENERATOR_H_
#include <OgreSceneManager.h>
#include "Zone.h"


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
	void createPlane(std::string pName);

	//Await further implementation
	void createTileMesh(Coordinate pPosition, std::string pName, Ogre::ColourValue pCol) const;
};

#endif