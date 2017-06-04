#include "LevelGenerator.h"
#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include "GameManager.h"
#include "math.h"
#include "Debug.h"

/// <summary>
/// Initializes a new instance of the <see cref="LevelGenerator"/> class.
/// </summary>
LevelGenerator::LevelGenerator() :
scalar(500)
{
	_zoneWidth = 10;
	_zoneDepth = 10;
	_maxCityWidth = 3;
	_maxCityDepth = 3;
	_maxCityAmount = 10;
	_maxTries = 750;

	Debug("Initializing zone");
	_zone[0] = Zone(_zoneWidth, _zoneDepth, _maxCityWidth, _maxCityDepth, _maxCityAmount, _maxTries, scalar);
	drawDungeonFloor(_zone[0], Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));

	determineCityTypes();
	_zone[0].printGrid();
	
	spawnCityContent();
}


/// <summary>
/// Finalizes an instance of the <see cref="LevelGenerator"/> class.
/// </summary>
LevelGenerator::~LevelGenerator()
{
}

/// <summary>
/// Restarts the level.
/// </summary>
void LevelGenerator::restart() {
	Debug("initializing zone");
	_zone[0] = Zone(_zoneWidth, _zoneDepth, _maxCityWidth, _maxCityDepth, _maxCityAmount, _maxTries, scalar);

	drawDungeonFloor(_zone[0], Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));
	_zone[0].printGrid();

	determineCityTypes();
	spawnCityContent();
}

/// returns empty position within dungeon
/// \param pEmptyNeighbours only returns positions with 8 emty neighbours
Coordinate LevelGenerator::getEmptyPosition(bool pEmptyNeighbours) {
	return _zone[0].getPosition(1, pEmptyNeighbours);
}

/// transfroms a world position to a grid coordinate
/// \param pWorldCoord coordinate in world position
Coordinate LevelGenerator::getGridPosition(Coordinate pWorldCoord) {

	int x = static_cast<int>((float)pWorldCoord.x / scalar + 0.5f);
	int z = static_cast<int>((float)pWorldCoord.z / scalar + 0.5f);

	return Coordinate(x, z);
}

/// transfroms a world position to a grid coordinate
/// \param pWorldCoord coordinate in world position
Coordinate LevelGenerator::getCollisionGridPosition(Coordinate pWorldCoord) {

	int x = static_cast<int>((float)pWorldCoord.x / (scalar / City::gridScalar) + 0.5f);
	int z = static_cast<int>((float)pWorldCoord.z / (scalar / City::gridScalar) + 0.5f);

	return Coordinate(x, z);
}

/// transforms a grid coordinate to a world position
/// \param pGridCoord grid coordinate
Ogre::Vector3 LevelGenerator::getWorldPosition(Coordinate pGridCoord) {
	return Ogre::Vector3((pGridCoord.x - 0.5f) * scalar, 0, (pGridCoord.z - 0.5f) * scalar);
}

/// transforms a grid coordinate to a world position
/// \param pGridCoord grid coordinate
Ogre::Vector3 LevelGenerator::getWorldPosition(RealCoordinate pGridCoord) {
	return Ogre::Vector3((pGridCoord.rx - 0.5f) * scalar, 0, (pGridCoord.rz - 0.5f) * scalar);
}

/// retrieve zone
/// \param pZoneId coordinate of zone, within zone grid
Zone LevelGenerator::getZone(Coordinate pZoneId) {
	//TODO:implement multiple zones
	return _zone[0];
}

/// retrieve zone
/// \param pX x position of the zone
/// \param pZ z position of the zone
Zone LevelGenerator::getZone(int pX, int pZ) {
	//TODO:implement multiple zones
	return _zone[0];
}

/// retrieve zone
/// \param pX x position of the zone
/// \param pZ z position of the zone
Zone* LevelGenerator::getZonePointer(int pX, int pZ) {
	//TODO:implement multiple zones
	return &_zone[0];
}

/// creates a tile for each position in the zone
/// \param pZone zone from which to draw the tiles
void LevelGenerator::drawDungeonFloor(Zone pZone, Ogre::ColourValue pCol) {
	createTileMesh("tileMesh", pCol);
	donaldTrump("wallMesh", Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));

	_dungeonNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("DungeonNode");

	for (int ix = 0; ix < pZone.getResolution().x; ++ix) {
		for (int iz = 0; iz < pZone.getResolution().z; ++iz) {
			if (pZone.getTile(ix, iz) > 0) {
				Ogre::SceneNode* tileNode = _dungeonNode->createChildSceneNode();
				tileNode->setPosition(ix * scalar - (scalar / 4.0f), 0, iz * scalar - (scalar / 4.0f));
				Ogre::SceneNode* wallNode = tileNode->createChildSceneNode();
				wallNode->setPosition((scalar / 2.0f), 0.0f, (scalar / 2.0f));

				std::stringstream name;
				name << "tile_" << ix << "-" << iz;

				if (!pZone.getTile(ix, iz - 1)) {
					Ogre::SceneNode* north = wallNode->createChildSceneNode();
					Ogre::Entity* nWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("northWall" + name.str(), "wallMesh");
					//north->yaw(Ogre::Radian(90 * Ogre::Math::PI / 180));
					nWallEntity->setMaterialName("InGame/Wall");
					north->attachObject(nWallEntity);
				}
				if (!pZone.getTile(ix - 1, iz)) {
					Ogre::SceneNode* east = wallNode->createChildSceneNode();
					Ogre::Entity* eWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("eastWall" + name.str(), "wallMesh");
					eWallEntity->setMaterialName("InGame/Wall");
					east->attachObject(eWallEntity);
					east->yaw(Ogre::Radian(90 * Ogre::Math::PI / 180));
				}
				if (!pZone.getTile(ix, iz + 1)) {
					Ogre::SceneNode* south = wallNode->createChildSceneNode();
					Ogre::Entity* sWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("southWall" + name.str(), "wallMesh");
					sWallEntity->setMaterialName("InGame/Wall");
					south->attachObject(sWallEntity);
					south->yaw(Ogre::Radian(180 * Ogre::Math::PI / 180));
				}
				if (!pZone.getTile(ix + 1, iz)) {
					Ogre::SceneNode* west = wallNode->createChildSceneNode();
					Ogre::Entity* wWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("westWall" + name.str(), "wallMesh");
					wWallEntity->setMaterialName("InGame/Wall");
					west->attachObject(wWallEntity);
					west->yaw(Ogre::Radian(-90 * Ogre::Math::PI / 180));
				}

				Ogre::Entity* tileEntity = GameManager::getSingleton().getSceneManager()->createEntity("entity: " + name.str(), "tileMesh");
				//testCity->setMaterialName("Test/ColourTest");
				tileEntity->setMaterialName("InGame/Floor");
				tileNode->attachObject(tileEntity);
			}
		}
	}
}

/// We're gonna build a wall!
/// \param pName unique name for retrieving the mesh object
/// \param pCol a colour modification to the object (white is normal colours)
void LevelGenerator::donaldTrump(std::string pName, Ogre::ColourValue pCol) const {
	Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual(pName, "General");

	Ogre::SubMesh* sub = mesh->createSubMesh();

	const float sqrt13(1 / 3);

	//create vertices
	const size_t nVertices = 4;
	const size_t vBufCount = 8 * nVertices;
	float vertices[vBufCount] = {
		static_cast<float>(scalar / 2.0f), static_cast<float>(0), -(scalar / 2.0f),			 //1
		sqrt13,  -sqrt13, -sqrt13,
		1,0,
		static_cast<float>(-(scalar / 2.0f)), static_cast<float>(0), -(scalar / 2.0f),				 //2
		-sqrt13, -sqrt13, -sqrt13,
		0,0,
		static_cast<float>(scalar/2.0f), static_cast<float>(scalar), -(scalar / 2.0f),       //3
		sqrt13,  -sqrt13, sqrt13,
		1,1,
		static_cast<float>(-(scalar / 2.0f)), static_cast<float>(scalar), -(scalar / 2.0f),			 //4
		-sqrt13, -sqrt13, sqrt13,
		0,1
	};

	Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
	Ogre::RGBA colours[nVertices];
	Ogre::RGBA* colorPtr = colours;


	rs->convertColourValue(pCol, colorPtr++); //0
	rs->convertColourValue(pCol, colorPtr++); //1
	rs->convertColourValue(pCol, colorPtr++); //2
	rs->convertColourValue(pCol, colorPtr);   //3

											  //indices
	const size_t iBufCount = 6;
	unsigned short faces[iBufCount] = {
		0, 2, 3,
		1, 0, 3
	};

	mesh->sharedVertexData = new Ogre::VertexData();
	mesh->sharedVertexData->vertexCount = nVertices;

	Ogre::VertexDeclaration* vDeclaration = mesh->sharedVertexData->vertexDeclaration;
	size_t offset = 0;
	//1th buffer
	//vertex description
	vDeclaration->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	vDeclaration->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	vDeclaration->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

	Ogre::HardwareVertexBufferSharedPtr vBuf =
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vBuf->writeData(0, vBuf->getSizeInBytes(), vertices, true);

	Ogre::VertexBufferBinding* bind = mesh->sharedVertexData->vertexBufferBinding;
	bind->setBinding(0, vBuf);

	//2nd buffer
	offset = 0;
	vDeclaration->addElement(1, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

	vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	vBuf->writeData(0, vBuf->getSizeInBytes(), colours, true);
	bind->setBinding(1, vBuf);

	Ogre::HardwareIndexBufferSharedPtr iBuf = Ogre::HardwareBufferManager::getSingleton().
		createIndexBuffer(
			Ogre::HardwareIndexBuffer::IT_16BIT,
			iBufCount,
			Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	iBuf->writeData(0, iBuf->getSizeInBytes(), faces, true);

	sub->useSharedVertices = true;
	sub->indexData->indexBuffer = iBuf;
	sub->indexData->indexCount = iBufCount;
	sub->indexData->indexStart = 0;

	mesh->_setBounds(Ogre::AxisAlignedBox(Ogre::AxisAlignedBox(-(scalar / 2.0f), 0.0f, -(scalar / 2.0f), scalar / 2.0f, scalar, -(scalar / 2.0f))));

	mesh->load();
}

/// <summary>
/// Spawns the content of the city.
/// </summary>
void LevelGenerator::spawnCityContent() {
	// loop through all cities
	for (int i = 0; i < _zone[0].cities.size(); ++i) {

		// set a pointer to the current city 
		City* thisCity = &_zone[0].cities[i];
		// switch on the city type
		switch (thisCity->TypeFlag()) {
		case CityRT:
			for (int j = 0; j < thisCity->Buildings().size(); ++j) { // for each building
				spawnNPCs(thisCity, &thisCity->Buildings()[j]);
			}

			break;
		case HideoutRT:
			// spawn enemy spawners in the middle of an enemy hideout
			spawnEnemy(thisCity, thisCity->Buildings()[0].residents);
			break;
		default:
			break;
		}
	}
	return;
}

/// <summary>
/// Spawns the NPCs for a specified building.
/// </summary>
/// <param name="pCity">The p city.</param>
/// <param name="pBuilding">The p this building.</param>
void LevelGenerator::spawnNPCs(City* pCity, Building* pBuilding) {
	for (int i = 0; i < pBuilding->residents; i++) {
		RealCoordinate npcCoord = pCity->getNpcPosition();
		if (npcCoord.rx < 0 || npcCoord.rz < 0) {
			Debug("not enough free space in city");
			break;
		}
		Ogre::Vector3 npcPos = Ogre::Vector3(npcCoord.rx, 18.0f, npcCoord.rz);

		//assign position to NPC
		Ogre::SceneNode* instanceNode = GameManager::getSingletonPtr()->getLevelManager()->getLevelNode()->createChildSceneNode();
		instanceNode->setPosition(npcPos);
		Ogre::Entity* instanceEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
		Ogre::SceneNode* rotationNode = instanceNode->createChildSceneNode();
		rotationNode->attachObject(instanceEntity);
		Npc* instanceScript = new Npc(instanceNode, rotationNode, instanceEntity, pCity, pBuilding);
	}
	pCity->removeNpcTiles();
}

/// <summary>
/// Places the enemy spawn node for the specified city.
/// </summary>
/// <param name="thisCity">The this city.</param>
/// <param name="i">The i.</param>
void LevelGenerator::spawnEnemy(City* pCity, int nAmount) {
	for (int i = 0; i < nAmount; i++) {
		RealCoordinate enemyCoord = pCity->getNpcPosition();
		if (enemyCoord.rx < 0 || enemyCoord.rz < 0) {
			//Debug("not enough free space in city")Z
			break;
		}
		Ogre::Vector3 enemyPos = Ogre::Vector3(enemyCoord.rx, 18.0f, enemyCoord.rz);

		//assign position to NPC
		Ogre::SceneNode* instanceNode = GameManager::getSingletonPtr()->getLevelManager()->getLevelNode()->createChildSceneNode();
		instanceNode->translate(enemyPos, Ogre::Node::TS_WORLD);
		Ogre::Entity* instanceEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("cube.mesh");
		Ogre::SceneNode* rotationNode = instanceNode->createChildSceneNode();
		rotationNode->attachObject(instanceEntity);
		BasicEnemy* instanceScript = new BasicEnemy(instanceNode, rotationNode, instanceEntity, pCity, GameManager::getSingletonPtr()->getLevelManager()->getCurrentLevel());
	}
	pCity->removeNpcTiles();
}

/// <summary>
/// Creates the tile mesh.
/// </summary>
/// <param name="pName">Name of the p.</param>
/// <param name="pCol">The p col.</param>
void LevelGenerator::createTileMesh(std::string pName, Ogre::ColourValue pCol) const {
	//TODO: add uv coordinates

	Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual(pName, "General");

	Ogre::SubMesh* sub = mesh->createSubMesh();

	const float sqrt13(1 / 3);

	//create vertices
	const size_t nVertices = 4;
	const size_t vBufCount = 8 * nVertices;
	float vertices[vBufCount] = {
		static_cast<float>(scalar), static_cast<float>(0), static_cast<float>(0),			 //1
		sqrt13,  -sqrt13, -sqrt13,
		0,0,
		static_cast<float>(0), static_cast<float>(0), static_cast<float>(0),				 //2
		-sqrt13, -sqrt13, -sqrt13,
		0,1,
		static_cast<float>(scalar), static_cast<float>(0), static_cast<float>(scalar),       //3
		sqrt13,  -sqrt13, sqrt13,
		1,0,
		static_cast<float>(0), static_cast<float>(0), static_cast<float>(scalar),			 //4
		-sqrt13, -sqrt13, sqrt13,
		1,1
	};

	Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
	Ogre::RGBA colours[nVertices];
	Ogre::RGBA* colorPtr = colours;


	rs->convertColourValue(pCol, colorPtr++); //0
	rs->convertColourValue(pCol, colorPtr++); //1
	rs->convertColourValue(pCol, colorPtr++); //2
	rs->convertColourValue(pCol, colorPtr);   //3

	//indices
	const size_t iBufCount = 6;
	unsigned short faces[iBufCount] = {
		0, 3, 2,
		1, 3, 0
	};

	mesh->sharedVertexData = new Ogre::VertexData();
	mesh->sharedVertexData->vertexCount = nVertices;

	Ogre::VertexDeclaration* vDeclaration = mesh->sharedVertexData->vertexDeclaration;
	size_t offset = 0;
	//1th buffer
	//vertex description
	vDeclaration->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	vDeclaration->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	vDeclaration->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

	Ogre::HardwareVertexBufferSharedPtr vBuf =
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	vBuf->writeData(0, vBuf->getSizeInBytes(), vertices, true);

	Ogre::VertexBufferBinding* bind = mesh->sharedVertexData->vertexBufferBinding;
	bind->setBinding(0, vBuf);

	//2nd buffer
	offset = 0;
	vDeclaration->addElement(1, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_COLOUR);

	vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
		offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	vBuf->writeData(0, vBuf->getSizeInBytes(), colours, true);
	bind->setBinding(1, vBuf);

	Ogre::HardwareIndexBufferSharedPtr iBuf = Ogre::HardwareBufferManager::getSingleton().
		createIndexBuffer(
			Ogre::HardwareIndexBuffer::IT_16BIT,
			iBufCount,
			Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	iBuf->writeData(0, iBuf->getSizeInBytes(), faces, true);

	sub->useSharedVertices = true;
	sub->indexData->indexBuffer = iBuf;
	sub->indexData->indexCount = iBufCount;
	sub->indexData->indexStart = 0;

	mesh->_setBounds(Ogre::AxisAlignedBox(0, 0, 0, scalar, 0, scalar));

	mesh->load();
}

/// <summary>
/// Determines the types for the cities.
/// </summary>
void LevelGenerator::determineCityTypes() {
	// if not two cities: problems!
	if(_zone[0].cities.size() < 3) {
		Debug("not enough cities to play this dungeon");
		return;
	}
	
	// start
	_startCity = &_zone[0].cities[0];
	_startCity->setType((int)CityRT);
	_zone[0].cities[1].setType((int)CityRT);

	// end
	City* furtherstCity = &_zone[0].cities[2];
	float dist = _startCity->getDistTo(furtherstCity);
	for (int i = 2; i < _zone[0].cities.size(); ++i) {
		_zone[0].cities[i].setType();

		if(_startCity->getDistTo(&_zone[0].cities[i]) > dist) {
			furtherstCity = &_zone[0].cities[i];
			dist = _startCity->getDistTo(&_zone[0].cities[i]);
		}
	}

	furtherstCity->setType((int)HideoutRT);

	_sisterNode = GameManager::getSingletonPtr()->getLevelManager()->getLevelNode()->createChildSceneNode("SisterNode");
	Ogre::Entity* _sis = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	_sisterNode->setScale(0.25f, 0.25f, 0.25f);
	_sisterNode->setPosition((furtherstCity->getCenterTile().x - 1) * scalar, 18.0f, (furtherstCity->getCenterTile().z) * scalar);
	_sis->setMaterialName("InGame/PinkHouse");

	_sisterNode->createChildSceneNode()->attachObject(_sis);

	_endCity = furtherstCity;

	for (int i = 0; i < _zone[0].cities.size(); ++i) {
		_zone[0].cities[i].init();
	}
}
