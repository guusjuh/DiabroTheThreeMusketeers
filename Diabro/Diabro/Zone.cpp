#include "Zone.h"
#include <ctime>
#include "GameManager.h"
#include "Coordinate.h"
#include "City.h"
#include <OgreConfigFile.h>
#include <OgreMath.h>
#include "Debug.h"

/// <summary>
/// Initializes a new instance of the <see cref="Zone"/> class.
/// </summary>
Zone::Zone() {
}

/// <summary>
/// Initializes a new instance of the <see cref="Zone"/> class.
/// </summary>
/// <param name="pWidth">Width of the zone.</param>
/// <param name="pDepth">The zone depth.</param>
/// <param name="pMaxCityWidth">Maximum width for the cities.</param>
/// <param name="pMaxCityHeight">Maximum height for the cities.</param>
/// <param name="pMaxCities">The maximum amount of cities.</param>
/// <param name="pMaxTries">The maximum amount of tries.</param>
/// <param name="pScalar">The scalar value.</param>
Zone::Zone(int pWidth, int pDepth, int pMaxCityWidth, int pMaxCityHeight, int pMaxCities, int pMaxTries, int pScalar) :
_width(pWidth), _depth(pDepth), _maxCityWidth(pMaxCityWidth), _maxCityHeight(pMaxCityHeight), _scalar(pScalar)
{	
	if (pWidth % 2 == 0 || pDepth % 2 == 0) {
		//zones use uneven sizes, this ensures walls can be created properly
		if (pWidth % 2 == 0) _width++;
		if (pDepth % 2 == 0) _depth++;
		Debug("zone sizes should be uneven, used: ", getResolution());
	}
	// create an empty grid for dungeon tiles
	_tiles = new int[_width * _depth];
	for (int ix = 0; ix < _width; ++ix) {
		for (int iy = 0; iy < _depth; ++iy) {
			setTile(ix, iy, 0);
		}
	}
	generateCities(pMaxTries, pMaxCities);
	int n = generatePathways(cities.size() + 1);
	Debug("connecting dungeon");
	connectDungeon(cities.size() + 1 + n, 0.5f);

	cleanGrid();
	collisionGridGenerated = false;
	
}

/// <summary>
/// Finalizes an instance of the <see cref="Zone"/> class.
/// </summary>
Zone::~Zone(){ }

City* Zone::getRandomCity(RoomType type) {
	std::vector<int> id;

	for (int i = 0; i < cities.size(); ++i) {
		if (cities[i].TypeFlag() == type) {
			id.push_back(i);
		}
	}

	int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, id.size() - 1);
	int x = id[randomroll];
	return &cities[x];
}

/// cleans the grid converting every number either to 1 or 0 based on their current value
void Zone::cleanGrid()
{
	for (int ix = 0; ix < _width; ++ix)
	{
		for (int iz = 0; iz < _depth; ++iz)
		{
			if (getTile(ix, iz) <= 0) setTile(ix, iz, 0); //set all positive values to 1 and negatives to 0
			else setTile(ix, iz, 1);
		}
	}
}

/// checks whether coordinate is within boundaries of the grid
/// \param pCoord coordinate to check
bool Zone::inGrid(Coordinate pCoord) {
	if (pCoord.x >= _width && pCoord.z >= _depth) return false;
	if (pCoord.x < 0 && pCoord.z < 0) return false;
	return true;
}

/// sets a tile to the given value
/// \param pX x coordinate of the tile
/// \param pZ z coordinate of the tile
/// \param pValue value to assign to the given tile 
void Zone::setTile(int pX, int pZ, int pValue) const {
	_tiles[pX + pZ * _width] = pValue;
}

/// sets a tile to the given value
/// \param pCoord coordinate of the tile
/// \param pValue value to assign to the given tile
void Zone::setTile(Coordinate pCoord, int pValue) const {
	_tiles[pCoord.x + pCoord.z * _width] = pValue;
}

/// returns the value of a given tile
/// \param pX x coordinate of the tile
/// \param pZ z coordinate of the tile
int Zone::getTile(int pX, int pZ) const {
	return _tiles[pX + pZ * _width];
}

/// returns the value of a given tile
/// \param pCoord coordinate of the tile
int Zone::getTile(Coordinate pCoord) const {
	return _tiles[pCoord.x + pCoord.z * _width];
}

/// finds an position of a certain value the dungeon
/// \param pMaxId value to look for
/// \param pCheckNeighbours should all eight neighbours have the same value?
Coordinate Zone::getPosition(int pId, bool pCheckNeighbours) {
	std::vector<Coordinate> positions;

	for (int ix = 1; ix < _width - 1; ++ix) {
		for (int iz = 1; iz < _depth - 1; ++iz) {
			if(getTile(ix, iz) == pId) {
				if (pCheckNeighbours) { // check neighbouring cell values
					if( (inGrid(Coordinate(ix - 1, iz - 1)) && getTile(ix - 1, iz - 1) == pId) &&
						(inGrid(Coordinate(ix    , iz - 1)) && getTile(ix    , iz - 1) == pId) &&
						(inGrid(Coordinate(ix + 1, iz - 1)) && getTile(ix + 1, iz - 1) == pId) &&
						(inGrid(Coordinate(ix - 1, iz    )) && getTile(ix - 1, iz    ) == pId) &&
						(inGrid(Coordinate(ix + 1, iz    )) && getTile(ix + 1, iz    ) == pId) &&
						(inGrid(Coordinate(ix - 1, iz + 1)) && getTile(ix - 1, iz + 1) == pId) &&
						(inGrid(Coordinate(ix    , iz + 1)) && getTile(ix    , iz + 1) == pId) &&
						(inGrid(Coordinate(ix + 1, iz + 1)) && getTile(ix + 1, iz + 1) == pId)) {
						positions.push_back(Coordinate(ix, iz));
					}
				}
				else {
					positions.push_back(Coordinate(ix, iz));
				}
			}
		}
	}
	if (positions.size() > 0) {
		int iRand = rand() % positions.size();
		return positions[iRand]; //return random position
	} else {
		return Coordinate(-1, -1); //return error coordinate
	}
	
}

int Zone::findPossibleConnections(City &c)
{
	int i = 0;
	for (int x = -1; x <= c.Width(); x++)
	{
		for (int z = -1; z <= c.Depth(); z++)
		{
			if (x == -1 || z == -1 || x == c.Width() || z == c.Depth())
			{
				if (getTile(c.Position().x + x, c.Position().z + z) == -1)
				{
					c.addConnection(c.Position() + Coordinate(x, z));
					i++;
				}
			}
		}
	}
	return i;
}

int Zone::findUsedConnections(City &c)
{
	c.clearConnections();
	for (int x = -1; x <= c.Width(); x++)
	{
		for (int z = -1; z <= c.Depth(); z++)
		{
			if (x == -1 || z == -1 || x == c.Width() || z == c.Depth())
			{
				if (getTile(c.Position().x + x, c.Position().z + z) > 0)
				{
					c.addConnection(Coordinate(c.Position().x + x, c.Position().z + z));
				}
				else
				{
					setTile(c.Position() + Coordinate(x, z), 0);
				}
			}
		}
	}
	return 0;
}

int Zone::getMaxValue()
{
	int max = 0;
	for (int x = 0; x < _width; x++)
	{
		for (int z = 0; z < _depth; z++)
		{
			if (max > getTile(x, z))
			{
				max = getTile(x, z);
			}
		}
	}
	return max;
}

/// connects rooms and pathways of the dungeon untill all routes are connected
/// \param pMaxId highest value used in the grid
/// \param pChance the decimal percentage of chance a room has to get an extra connection to the dungeon
void Zone::connectDungeon(int pMaxId, float pChance) {
	//TODO: find all possible connections
	Debug("city loop");
	std::vector<Coordinate> connections; ///< holds all possible connections
	for (int i = 0; i < cities.size(); i++)
	{
		//1. collect all connections positions and set their value on -1
		getPossibleConnections(cities[i], &connections);
		//2. walk through all connections per city and add all posibilities
		int n = findPossibleConnections(cities[i]);

		//3. Open one connection per room
		int rnd[2];
		rnd[0] = rand() % n;
		Coordinate picked = cities[i].getConnection(rnd[0]);
		setTile(picked, 1);

		//4. open up MOAAARRR connections
		if (cities[i].nConnections() > 1) {
			//check whether an optional connection should be opened up
			float rndPercent = (rand() % 100 + 1) / 100.0f;
			if (rndPercent <= pChance) {
				rnd[1] = rand() % n;
				while (rnd[0] == rnd[1]) {
					rnd[1] = rand() % n;
				}

				setTile(cities[i].getConnection(rnd[1]), 1);
			}
		}
		findUsedConnections(cities[i]);
	}

	std::vector<Coordinate> options;
	int regions = changeTileValues(pMaxId);
	printGrid();
	//5. connect the still separated dungeon parts

	for (int i = 2; i < regions + 1; i++)
	{		
		for (int j = 0; j < connections.size(); j++)
		{
			//find all used connections and remove them from the list
			if (getTile(connections[j]) > 0)
			{
				connections.erase(connections.begin() + j);
				j--;
			} else //1. find all positions with an higher number next to them
			{
				Coordinate top, bot, left, right;
				top = connections[j] + Coordinate(0, 1);
				bot = connections[j] + Coordinate(0, -1);
				left = connections[j] + Coordinate(-1, 0);
				right = connections[j] + Coordinate(1, 0);
				int bar = 0;

				if ((getTile(top) > 0 && getTile(bot) > 0) && (getTile(top) == i || getTile(bot) == i)
					&& getTile(bot) != getTile(top))
				{
					options.push_back(connections[j]);
					connections.erase(connections.begin() + j);
					j--;

				}
		Debug("changeTileValues 2");

				if ((getTile(left) > 0 && getTile(right) > 0) && (getTile(left) == i || getTile(right) == i)
					&& getTile(right) != getTile(left))
				{
					options.push_back(connections[j]);
					connections.erase(connections.begin() + j);
					j--;

				}
			}
		}
		//options = all connections connecting i and something not i
		//2. open some of these connections
		for (int k = 0; k < 3; k++)
		{
			if (options.size() > 1)
			{
				int rnd = rand() % options.size();
				setTile(options[rnd], 1);
				options.erase(options.begin() + rnd);
			}
		}
		//3. repeat for each separate region
		// WARNING: check if connection even exist, else use another region
	}

			Debug("changeTileValues 3");
	regions = changeTileValues(pMaxId);
	if (regions <= 0)
	{
		printGrid();
	}
	while (regions > 1)
	{
		int rnd[5];
		for (int i = 0; i < 5; i++)
		{
			int index = rnd[i];
			setTile(options[index], 1);
		}
		regions = changeTileValues(getMaxValue());
		Debug("-", regions);
	}

	printGrid();
	
	Debug("regions last:", regions);
	for (int i = 0; i < cities.size(); i++)
	{
		findUsedConnections(cities[i]);
	}
	printGrid();
	int i = 0;
}

///prints all values used in the grid (Windows only method)
void Zone::printValues() {
	int values[100];

	for (int ix = 0; ix < _width; ++ix) {
		for (int iz = 0; iz < _depth; ++iz) {
			int n = getTile(ix, iz);
			if (n > 0) {
				values[n] = n;
			}
		}
	}
	for (int i = 1; i < 100; ++i) {
		if (values[i] > 0) {
			Debug(static_cast<float>(values[i]));
		}
		
	}
}

/// returns the width and depth of the zone
Coordinate Zone::getResolution() const {
	return(Coordinate(_width, _depth));
}

///checks all tiles and changes the id of connected regions to the lowest id of the two, returns amount of remaining regions
/// \param pMaxIndex maximum index value used in the whole zone
int Zone::changeTileValues(int pMaxIndex) {
	
	int currentRegion = 1;
	int amountOfRegions = 0;
	for (int i = 1; i <= pMaxIndex; ++i) {
		std::vector<Coordinate> positions;
		//pick random cell for current region
		positions.push_back(getPosition(currentRegion, false));
		
		if (!inGrid(positions[0]) || getTile(positions[0]) != currentRegion) { //continue if no options are found for this id
			positions.clear();
			currentRegion++;
			continue;
		}

		if (positions[0].x >= 0 && positions[0].z >= 0) {
			while(!positions.empty()) {
				//find all neighbours (which are >0) and add them to the list
				if (positions[0].z + 1 < _depth - 1) {
					if (getTile(positions[0].x, positions[0].z + 1) > 0) positions.push_back(Coordinate(positions[0].x, positions[0].z + 1));
				}
				if (positions[0].x + 1 < _width - 1) {
					if (getTile(positions[0].x + 1, positions[0].z) > 0) positions.push_back(Coordinate(positions[0].x + 1, positions[0].z));
				}
				if (positions[0].z > 0) {
					if (getTile(positions[0].x, positions[0].z - 1) > 0) positions.push_back(Coordinate(positions[0].x, positions[0].z - 1));
				}
				if (positions[0].x - 1 > 0) {
					if (getTile(positions[0].x - 1, positions[0].z) > 0) positions.push_back(Coordinate(positions[0].x - 1, positions[0].z));
				}
				
				//assigns temporary value to cell (negative of id value, one is not used as negative one is used for possible connections)
				setTile(positions[0], -(amountOfRegions + 2));
				positions.erase(positions.begin());
			}
			amountOfRegions++;
		}
		positions.clear();
		currentRegion++;
	}
	for (int ix = 0; ix < _width; ++ix) {
		for (int iz = 0; iz < _depth; ++iz) {
			int n = getTile(ix, iz);
			if (n < 0 && n != -1) {
				// set tiles to correct values
				setTile(ix, iz, getTile(ix, iz) * -1);
			}
		}
	}
	return amountOfRegions;
}

/// finds all possible points to connect a city with a path
/// \param pCity the city for which connections have to be found
/// \param pConnections pointer to connections vector existing of a position and city id
int Zone::getPossibleConnections(City pCity, std::vector<Coordinate> *pConnections) {
	int connectionAmount = 0;
	for (int ix = 0; ix < pCity.Width(); ++ix) {
		for (int iz = 0; iz < pCity.Depth(); ++iz) {
			//north
			if (getTile(pCity.Position().x + ix, pCity.Position().z + iz - 1) == 0 && pCity.Position().z + iz - 2 > 0) {
				if (getTile(pCity.Position().x + ix, pCity.Position().z + iz - 2) > 0) {
					//save connection in grid and connection vector
					pConnections->push_back(Coordinate(pCity.Position().x + ix, pCity.Position().z + iz - 1));
					setTile(pCity.Position().x + ix, pCity.Position().z + iz - 1, -1);
					connectionAmount++;
				}
			}
			//east
			if (getTile(pCity.Position().x + ix + 1, pCity.Position().z + iz) == 0 && pCity.Position().x + ix + 2 < _width) {
				if (getTile(pCity.Position().x + ix + 2, pCity.Position().z + iz) > 0) {
					//save connection in grid and connection vector
					pConnections->push_back(Coordinate(pCity.Position().x + ix + 1, pCity.Position().z + iz));
					setTile(pCity.Position().x + ix + 1, pCity.Position().z + iz, -1);
					connectionAmount++;
				}
			}
			//south
			if (getTile(pCity.Position().x + ix, pCity.Position().z + iz + 1) == 0 && pCity.Position().z + iz + 2 < _depth) {
				if (getTile(pCity.Position().x + ix, pCity.Position().z + iz + 2) > 0) {
					//save connection in grid and connection vector
					pConnections->push_back(Coordinate(pCity.Position().x + ix, pCity.Position().z + iz + 1));
					setTile(pCity.Position().x + ix, pCity.Position().z + iz + 1, -1);
					connectionAmount++;
				}
			}
			//west
			if (getTile(pCity.Position().x + ix - 1, pCity.Position().z + iz) == 0 && pCity.Position().x + ix - 2 > 0) {
				if (getTile(pCity.Position().x + ix - 2, pCity.Position().z + iz) > 0) {
					//save connection in grid and connection vector
					pConnections->push_back(Coordinate(pCity.Position().x + ix - 1, pCity.Position().z + iz));
					setTile(pCity.Position().x + ix - 1, pCity.Position().z + iz, -1);
					connectionAmount++;
				}
			}
		}
	}
	return connectionAmount; //returns length of values added to the connection vector
}

/// generates pathways within the generated grid
/// \param pPathId id to start with when generating the path (same as amount of cities)
int Zone::generatePathways(int pPathId) {
	int nIterations = 0;

	while (!checkGrid()) {
		std::vector<Coordinate> cellList;

		//Possible improvement: pass startpos to method to use heuristic for shapes
		//get random empty position
		Coordinate position = getPosition(0, true);
		
		//only use uneven values, paths carve two tiles and only make corners on uneven values
		//even values will cause paths and rooms to collide
		if (position.x % 2 == 0) {
			(position.x + 2 >= _width) ? position.x-- : position.x++;
		}
		if (position.z % 2 == 0) {
			(position.z + 2 >= _depth) ? position.z-- : position.z++;
		}

		cellList.push_back(Coordinate(position.x, position.z));
		int current = 0;
		setTile(cellList[current].x, cellList[current].z, -1); // all currently used cells are marked as -1

		while (!cellList.empty()) { //find each cells neighbours, add them to the list, remove current. Repeat until list is empty
			if (hasNeighBours(cellList[current], 2)) {
				//TODO: select all neighbours at once
				cellList.push_back(getRndNeighbour(cellList[current], pPathId + nIterations));
				++current;
				setTile(cellList[current].x, cellList[current].z, -1);
			}
			else {
				setTile(cellList[current].x, cellList[current].z, pPathId + nIterations);
				cellList.erase(cellList.begin() + current);
				--current;
				//TODO: implement heuristic types for new current cell
			}
		}
		nIterations++; // returns amount of separate paths
	}
	return nIterations;
}

/// finds a random neighbour of pCell
/// \param pCell selected cell
/// \param pPathId id of the drawn path (will be the new id of the cell)
Coordinate Zone::getRndNeighbour(Coordinate pCell, int pPathId) {
	std::vector<Coordinate> neighbours = getNeighbours(pCell);
	int i = rand() % neighbours.size();
	Coordinate difference = neighbours[i];
	
	difference.x = difference.x - pCell.x;
	difference.z = difference.z - pCell.z;

	//mark the cell in between current and next cell as drawn
	if (difference.x == 0) {
		(difference.z > 0) ? setTile(pCell.x, pCell.z + 1, pPathId) : setTile(pCell.x, pCell.z - 1, pPathId);
	}
	else {
		(difference.x > 0) ? setTile(pCell.x + 1, pCell.z, pPathId) : setTile(pCell.x - 1, pCell.z, pPathId);
	}	

	return (Coordinate(neighbours[i]));	
}

/// finds all neighbours of a cell
/// \param pCell selected cell
std::vector<Coordinate> Zone::getNeighbours(Coordinate pCell) {
	std::vector<Coordinate> neighbours;
	if (pCell.z + 2 < _depth && !getTile(pCell.x, pCell.z + 2)) neighbours.push_back(Coordinate(pCell.x, pCell.z + 2));
	if (pCell.x + 2 < _width && !getTile(pCell.x + 2, pCell.z)) neighbours.push_back(Coordinate(pCell.x + 2, pCell.z));
	if (pCell.x - 2 >= 0 && !getTile(pCell.x - 2, pCell.z)) neighbours.push_back(Coordinate(pCell.x - 2, pCell.z));
	if (pCell.z - 2 >= 0 && !getTile(pCell.x, pCell.z - 2)) neighbours.push_back(Coordinate(pCell.x, pCell.z - 2));
	return (neighbours);
}

/// checks whether a cell has neighbours
/// \param pCell selected cell
/// \param pDistance checks for neighbours 'x' cells away, skipping cells in between
bool Zone::hasNeighBours(Coordinate pCell, int pDistance) {
	if (pCell.z + pDistance < _depth - 1 && !getTile(pCell.x, pCell.z + pDistance)) return true;
	if (pCell.x + pDistance < _width - 1 && !getTile(pCell.x + pDistance, pCell.z)) return true;
	if (pCell.x - pDistance >= 0 && !getTile(pCell.x - pDistance, pCell.z)) return true;
	if (pCell.z - pDistance >= 0 && !getTile(pCell.x, pCell.z - pDistance)) return true;
	return false;
}

/// checks whether a position is already taken
/// \param pPosition position to check
bool Zone::hasCollision(Coordinate pPosition) {
	if (inGrid(pPosition))return true;
	return(getTile(pPosition.x, pPosition.z) ? true : false);
}

/// checks wheter a city can be placed on its position
/// \param pC selected city
bool Zone::hasCollision(City pC) {
	if (pC.Position().x + pC.Width() >= _width || pC.Position().x <= 0) return true;
	if (pC.Position().z + pC.Depth() >= _depth || pC.Position().z <= 0) return true;

	for (int ix = (pC.Position().x > 0)? -1: 0; ix < pC.Width() + 1; ++ix) {
		for (int iz = (pC.Position().z > 0)? -1: 0; iz < pC.Depth() + 1; ++iz) {
			if (getTile(pC.Position().x + ix, pC.Position().z + iz)) return true;
		}
	}
	return false;
}

/// checks if all values in the grid are filled (exept for wall positions)
bool Zone::checkGrid() {
	for (int iX = 1; iX < _width; iX += 2) {
		for (int iZ = 1; iZ < _depth; iZ += 2) {
			if (getTile(iX, iZ) == 0) {
				return false;
			}
		}
	}
	return true;
}


/// places a city, if the position has not been taken yet otherwise it returns false
/// \param pC selected city
bool Zone::placeCity(City pC) {
	if (!hasCollision(pC)) {
		for (int ix = 0; ix < pC.Width(); ++ix) {
			for (int iz = 0; iz < pC.Depth(); ++iz) {
				setTile(pC.Position().x + ix, pC.Position().z + iz, pC.ID());
			}
		}
		//pC.init();
		cities.push_back(pC);
		return true;
	}
	return false;
}

/// generates cities by trying to place them on random positions
/// \param pMaxTries maximum amount of tries before the algorithm quits
/// \param pMaxCities the maximum amount of cities to place
void Zone::generateCities(int pMaxTries, int pMaxCities) {
	int nCities = 0;
	seed = time(NULL);

	srand(seed);

	for (int iTry = 0; iTry < pMaxTries; ++iTry) {
		//generate width, depth, should be uneven for wall creation
		int width = rand() % (_maxCityWidth - 2) + 2;
		int depth = rand() % (_maxCityHeight - 2) + 2;
		(width % 2 == 0) ? width++ : width;
		(depth % 2 == 0) ? depth++ : depth;

		//generate position, should be uneven for wall creation
		int x = rand() % (_width - width - 1) + 1;
		int z = rand() % (_depth - depth - 1) + 1;
		(x % 2 == 0) ? x++ : x;
		(z % 2 == 0) ? z++ : z;

		//try to place the city
		if (placeCity(City(x, z, width, depth, nCities + 1, _scalar))) {
			++nCities;
		}
		if (nCities >= pMaxCities) {
			break;
		}
	}
}

/// prints all values of the grid (windows only method)
void Zone::printGrid() {

	Debug d = Debug();
	d.printArray(_tiles, _width, _width * _depth);
	printf("\n");
}

/// <summary>
/// Obtains the collision grid.
/// </summary>
/// <returns></returns>
bool* Zone::getCollisionGrid() {
	if (!collisionGridGenerated){
		//create the array
		collisionGrid = generateCollisionGrid();
		collisionGridGenerated = true;
		Debug("Collision grid:");
		printCollisionGrid();
		int i = 0;
	}
	return collisionGrid;
}

/// <summary>
/// Generates the collision grid.
/// </summary>
/// <returns></returns>
bool* Zone::generateCollisionGrid() {
	bool* grid = new bool[(_width * City::gridScalar * _depth * City::gridScalar)];

	for (size_t i = 0; i < _width * City::gridScalar; i++)
	{
		for (size_t j = 0; j < _depth * City::gridScalar; j++)
		{
			if (getTile((i / City::gridScalar), (j / City::gridScalar)) > 0){
				grid[i + j * _width * City::gridScalar] = true;
			}
			else
			{
				grid[i + j * _width * City::gridScalar] = false;
			}
		}
	}
	for (size_t i = 0; i < cities.size(); i++)
	{
		//TODO: go through city tiles
		//1. double for-loop
		for (int x = 0; x < cities[i].scaledWidth(); x++)
		{
			for (int z = 0; z < cities[i].scaledDepth(); z++)
			{
				if (cities[i].getTile(x, z))
				{
					Coordinate position = cities[i].Position();
					position.x *= City::gridScalar;
					position.z *= City::gridScalar;
					position = position + Coordinate(x, z);

					grid[position.x + position.z * _width * City::gridScalar] = false;
				}
			}
		}
		
		
		/*std::vector<Coordinate> buildings = cities[i].buildingPositions();
		for (size_t j = 0; j < buildings.size(); j++)
		{
			grid[buildings[j].x + buildings[j].z * _width * City::gridScalar] = false;// set tiles at building pos false
		}*/
	}
	

	return grid;
}

/// <summary>
/// Prints the collision grid.
/// </summary>
void Zone::printCollisionGrid() {
	Debug d = Debug();
	d.printArray(collisionGrid, _width * City::gridScalar, _width * City::gridScalar * _depth * City::gridScalar);
}