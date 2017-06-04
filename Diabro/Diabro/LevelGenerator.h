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

/// <summary>
/// The class that generates and stores the dungeon. 
/// </summary>
class LevelGenerator
{
	friend class LevelManager;
public:
	LevelGenerator();
	~LevelGenerator();

	void restart();

	Zone getZone(Coordinate pZoneId);
	Zone getZone(int pX, int pZ);
	Zone* getZonePointer(int pX, int pZ);
	
	Ogre::Vector3 getWorldPosition(Coordinate pWorldCoord);
	Ogre::Vector3 getWorldPosition(RealCoordinate pWorldCoord);
	Coordinate getGridPosition(Coordinate pGridCoord);
	Coordinate getCollisionGridPosition(Coordinate pGridCoord);
	Coordinate getEmptyPosition(bool pEmptyNeighbours);
	
	float getDistToSis(Ogre::Vector3 otherPos) {
		return sqrt(abs(pow((_sisterNode->getPosition() - otherPos).x, 2)
			+ pow((_sisterNode->getPosition() - otherPos).z, 2)));
	}
	Ogre::Vector3 getSisPos() {
		return _sisterNode->getPosition();
	}

	Ogre::Vector3 getNorth() {
		return getZone(0, 0).getCenterZone() + Ogre::Vector3(0, 0, -100000000.0f);
	}
	
	Ogre::Vector3 getSouth() {
		return getZone(0, 0).getCenterZone() + Ogre::Vector3(0, 0, +100000000.0f);
	}

	Ogre::Vector3 getStartPos() {
		return getWorldPosition(_startCity->getCenterPosition());
	}

	City* getEndCity() { return _endCity; }

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
	void spawnEnemy(City*, int);
	void spawnNPCs(City*, Building*);
	
	Ogre::SceneNode* _dungeonNode;
	std::vector<Ogre::Entity*> _content;

	int _zoneWidth;
	int _zoneDepth;
	int _maxCityWidth;
	int _maxCityDepth;
	int _maxCityAmount;
	int _maxTries;

};

#endif