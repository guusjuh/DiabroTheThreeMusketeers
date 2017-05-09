#ifndef LEVEL_GENERATOR_H_
#define LEVEL_GENERATOR_H_
#include <OgreSceneManager.h>
#include "Zone.h"


class LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	Zone getZone(int pX, int pZ);
	int scalar;

private:
	Zone _zone[1];

	void drawDungeonFloor(int pScalar, Zone pZone);
	void createPlane(int pScalar, std::string pName);

	//Await further implementtation
	//void createTileMesh(int pScalar, Coordinate pPosition, std::string pName);
	//void createCityMesh(City pCity, int scalar, std::string pName, Ogre::ColourValue pColour) const;
	//void placeCity(City pCity, std::string pName, Ogre::ColourValue pColour) const;
};

#endif