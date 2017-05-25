#include "LevelGenerator.h"
#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include "GameManager.h"
#include "math.h"

LevelGenerator::LevelGenerator():
scalar(200)
{
	debug("Initializing zone");
	_zone[0] = Zone(30, 30, 6, 6, 10, 100, scalar);

	debug("generating geometry", 1);
	drawDungeonFloor(_zone[0], Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));

	debug("determine city types");
	determineCityTypes();
	_zone[0].printGrid();
	debug("spawning content");
	spawnCityContent();
}


LevelGenerator::~LevelGenerator()
{
}

void LevelGenerator::restart() {
	debug("Initializing zone");
	_zone[0] = Zone(30, 30, 6, 6, 10, 100, scalar);
	
	debug("generating geometry", 1);
	drawDungeonFloor(_zone[0], Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));
	_zone[0].printGrid();

	debug("determine city types");
	determineCityTypes();
	debug("spawning content");
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

	return Coordinate(x,z);
}

/// transforms a grid coordinate to a world position
/// \param pGridCoord grid coordinate
Ogre::Vector3 LevelGenerator::getWorldPosition(Coordinate pGridCoord) {
	return Ogre::Vector3((pGridCoord.x - 0.5f) * scalar, 0, (pGridCoord.z - 0.5f) * scalar);
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
	donaldTrump("wallMesh", Ogre::ColourValue(0.0f, 1.0f, 0.0f, 1.0f));

	_dungeonNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("DungeonNode");
	debug("dungeonx:", _dungeonNode->getPosition().x);
	debug("dungeony:", _dungeonNode->getPosition().y);
	debug("dungeonz:", _dungeonNode->getPosition().z);

	for (int ix = 0; ix < pZone.getResolution().x; ++ix) {
		for (int iz = 0; iz < pZone.getResolution().z; ++iz) {
			if (pZone.getTile(ix, iz) > 0) {
				Ogre::SceneNode* thisSceneNode = _dungeonNode->createChildSceneNode();
				thisSceneNode->setPosition(ix * scalar, 0, iz * scalar);
				Ogre::SceneNode* wallNode = thisSceneNode->createChildSceneNode();
				wallNode->setPosition(Ogre::Vector3(scalar / 2.0f, 0, scalar / 2.0f));

				std::stringstream name;
				name << "tile_" << ix << "-" << iz;

				if (!pZone.getTile(ix, iz - 1)) {
					Ogre::SceneNode* north = wallNode->createChildSceneNode();
					Ogre::Entity* nWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("northWall" + name.str(), "wallMesh");
					//north->yaw(Ogre::Radian(90 * Ogre::Math::PI / 180));
					nWallEntity->setMaterialName("Examples/Rockwall");
					north->attachObject(nWallEntity);
				}
				if (!pZone.getTile(ix - 1, iz)) {
					Ogre::SceneNode* east = wallNode->createChildSceneNode();
					Ogre::Entity* eWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("eastWall" + name.str(), "wallMesh");
					eWallEntity->setMaterialName("Examples/Rockwall");
					east->attachObject(eWallEntity);
					east->yaw(Ogre::Radian(90 * Ogre::Math::PI / 180));
				}
				if (!pZone.getTile(ix, iz + 1)) {
					Ogre::SceneNode* south = wallNode->createChildSceneNode();
					Ogre::Entity* sWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("southWall" + name.str(), "wallMesh");
					sWallEntity->setMaterialName("Examples/Rockwall");
					south->attachObject(sWallEntity);
					south->yaw(Ogre::Radian(180 * Ogre::Math::PI / 180));
				}
				if (!pZone.getTile(ix + 1, iz)) {
					Ogre::SceneNode* west = wallNode->createChildSceneNode();
					Ogre::Entity* wWallEntity = GameManager::getSingleton().getSceneManager()->createEntity("westWall" + name.str(), "wallMesh");
					wWallEntity->setMaterialName("Examples/Rockwall");
					west->attachObject(wWallEntity);
					west->yaw(Ogre::Radian(-90 * Ogre::Math::PI / 180));
				}

				Ogre::Entity* zoneEntity = GameManager::getSingleton().getSceneManager()->createEntity("entity: " + name.str(), "tileMesh");
				//testCity->setMaterialName("Test/ColourTest");
				zoneEntity->setMaterialName("Examples/Rockwall");
				thisSceneNode->attachObject(zoneEntity);
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

	mesh->_setBounds(Ogre::AxisAlignedBox(Ogre::AxisAlignedBox::EXTENT_INFINITE));

	mesh->load();
}

void LevelGenerator::spawnCityContent() {
	// loop through all cities
	for (int i = 0; i < _zone[0].cities.size(); ++i) {

		// set a pointer to the current city 
		City* thisCity = &_zone[0].cities[i];
		// switch on the city type
		switch (thisCity->typeFlag) {
		case CityRT:
			for (int j = 0; j < thisCity->Buildings().size(); ++j) { // for each building
				spawnNPCs(thisCity, &thisCity->Buildings()[j]);
			}

			break;
		case HideoutRT:
			// spawn enemy spawners in the middle of an enemy hideout
			placeEnemySpawnNode(thisCity, i);
			break;
		default:
			break;
		}
	}
	return;
}
void LevelGenerator::spawnNPCs(City* pCity, Building* pThisBuilding) {
	// catch the buildings position
	Ogre::Vector3 buildingPosition = pThisBuilding->getPositionInFrontOf();
	Ogre::Vector3 offsets[3];
	offsets[0] = Ogre::Vector3(-100, 25, -50);
	offsets[1] = Ogre::Vector3(-100, 25, 0);
	offsets[2] = Ogre::Vector3(-100, 25, 50);
	// for each resident
	for (int i = 0; i < pThisBuilding->residents; ++i) {
		// the scene node for the resident
		Ogre::SceneNode* instanceNode = GameManager::getSingletonPtr()->getLevelManager()->getLevelNode()->createChildSceneNode();
		instanceNode->translate(buildingPosition + offsets[i], Ogre::Node::TS_WORLD);
		Ogre::Entity* instanceEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("penguin.mesh");
		Ogre::SceneNode* rotationNode = instanceNode->createChildSceneNode();
		rotationNode->attachObject(instanceEntity);
		Npc* instanceScript = new Npc(instanceNode, rotationNode, instanceEntity, pCity, pThisBuilding);
		//instanceScript->initialize();
	}
}
void LevelGenerator::placeEnemySpawnNode(City* thisCity, int i) {
	Ogre::SceneNode* enemySpawnerNode = GameManager::getSingletonPtr()->getLevelManager()->getLevelNode()->createChildSceneNode("enemySpawn" + i);
	Spawner<BasicEnemy>* enemySpawner = new Spawner<BasicEnemy>(enemySpawnerNode, 3, Ogre::Vector3((thisCity->position.x + thisCity->width / 2) * scalar, 0, (thisCity->position.z + thisCity->depth / 2) * scalar), &_zone[0].cities[i]);
}

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

void LevelGenerator::determineCityTypes() {
	// if not two cities: problems!
	if(_zone[0].cities.size() < 2) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		printf("Not enough cities to play this dungeon!");
		fclose(fp);
#endif
		return;
	}
	
	// start
	_startCity = &_zone[0].cities[0];
	_startCity->setType((int)CityRT);

	// end
	City* furtherstCity = &_zone[0].cities[1];
	float dist = _startCity->getDistTo(furtherstCity);
	for (int i = 1; i < _zone[0].cities.size(); ++i) {
		_zone[0].cities[i].setType();

		if(_startCity->getDistTo(&_zone[0].cities[i]) > dist) {
			furtherstCity = &_zone[0].cities[i];
			dist = _startCity->getDistTo(&_zone[0].cities[i]);
		}
	}

	furtherstCity->setType((int)HideoutRT);

	_sisterNode = GameManager::getSingletonPtr()->getLevelManager()->getLevelNode()->createChildSceneNode("TesterNode");
	Ogre::Entity* _sis = GameManager::getSingletonPtr()->getSceneManager()->createEntity("ninja.mesh");
	_sisterNode->createChildSceneNode()->attachObject(_sis);
	_sisterNode->setPosition(Ogre::Vector3(furtherstCity->getCenterTile().x * scalar, 0, furtherstCity->getCenterTile().z* scalar));

	_endCity = furtherstCity;
	debug("end pos x:", furtherstCity->position.x);
	debug("end pos z:", furtherstCity->position.z);

	for (int i = 0; i < _zone[0].cities.size(); ++i) {
		_zone[0].cities[i].init();
	}
}
