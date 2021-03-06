#ifndef ZONE_H_
#define ZONE_H_

#include "City.h"
#include <vector>

/// <summary>
/// A part of the dungeon. 
/// </summary>
class Zone
{
	friend class LevelAccess;
protected:
	int _maxCityWidth;
	int _maxCityHeight;

	int* _tiles; ///< holds the zone grid, filled with all zone positions
	int _numberOfRegions; ///< not yet implemented, decides the amount of closed of regions within the zone

	unsigned int seed;
	unsigned int lastSeed;

	bool* collisionGrid;
	bool collisionGridGenerated;


public:
	int _width;
	int _depth;

	const static int scalar;

	City* getRandomCity(RoomType type);// { return &cities[0]; }
	Ogre::Vector3 getCenterZone() { return Ogre::Vector3(_width * scalar / 2.0f, 0, _depth * scalar / 2.0f); }

	std::vector<City> cities; ///< vector containing all cities within the zone

	Zone(int pWidth, int pHeight, int pMaxCityWidth, int pMaxCityHeight, int pMaxCities, int pMaxTries);
	Zone();
	~Zone();

	void setTile(int pX, int pZ, int pValue) const;
	void setTile(Coordinate pCoord, int pValue) const;
	int getTile(int pX, int pZ) const;
	int getTile(Coordinate pCoord) const;
	Coordinate getResolution() const;
	
	Coordinate getPosition(int pId, bool pCheckNeighbours);
	void printGrid();

	void printCollisionGrid();
	bool* getCollisionGrid();

protected:
	void cleanGrid();
	bool inGrid(Coordinate pCoord);

	int findPossibleConnections(City &c);
	int findUsedConnections(City &c);
	void connectDungeon(int pId, float pChance);
	int getPossibleConnections(City pCity, std::vector<Coordinate> *pConnections);
	int changeTileValues(int pMaxIndex);
	int generatePathways(int pPathId);
	std::vector<Coordinate> getNeighbours(Coordinate pCell);
	Coordinate getRndNeighbour(Coordinate pCell, int nPathId);
	bool hasNeighBours(Coordinate pCell, int pDistance);
	bool checkGrid();

	int getMaxValue();
	void printValues();
	
	void generateCities(int pMaxTries, int pMaxCities);
	void generateCity(int& nCities);
	bool hasCollision(Coordinate pPosition);
	bool hasCollision(City pC);
	bool placeCity(City pC);

	bool* generateCollisionGrid();
};

#endif