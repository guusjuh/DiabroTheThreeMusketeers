#include "LevelGenerator.h"
#include <OgreMeshManager.h>
#include <OgreSubMesh.h>
#include "GameManager.h"
#include "math.h"

LevelGenerator::LevelGenerator():
scalar(1000)
{
	//create zone and generate dungeon
	_zone[0] = Zone(10, 10, 5, 5, 10, 500);
	
	drawDungeonFloor(getZone(0, 0), Ogre::ColourValue(1.0f, 1.0f, 0.0f, 1.0f));
}

LevelGenerator::~LevelGenerator()
{
}
/// returns empty position within dungeon
/// \param pEmptyNeighbours only returns positions with 8 emty neighbours
Coordinate LevelGenerator::getEmptyPosition(bool pEmptyNeighbours) {
	return _zone[0].getPosition(1, pEmptyNeighbours);
}

/// transfroms a world position to a grid coordinate
/// \param pWorldCoord coordinate in world position
Coordinate LevelGenerator::getGridPosition(Coordinate pWorldCoord) {
	int x = static_cast<int>(ceil(pWorldCoord.x / scalar + 0.0f));
	int z = static_cast<int>(ceil(pWorldCoord.z / scalar + 0.0f));

	return Coordinate(x,z);
}

/// transforms a grid coordinate to a world position
/// \param pGridCoord grid coordinate
Coordinate LevelGenerator::getWorldPosition(Coordinate pGridCoord) {
	return Coordinate(pGridCoord.x * scalar, pGridCoord.z * scalar);
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
/// creates a tile for each position in the zone

/// \param pZone zone from which to draw the tiles
void LevelGenerator::drawDungeonFloor(Zone pZone, Ogre::ColourValue pCol) {
	Ogre::SceneNode* thisSceneNode = GameManager::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	thisSceneNode->setPosition(0, 0, 0);

	for (int ix = 0; ix < pZone.getResolution().x; ++ix) {
		for (int iz = 0; iz < pZone.getResolution().z; ++iz) {
			if (pZone.getTile(ix, iz) > 0) {
				std::stringstream name;
				name << "tile_" << ix << "-" << iz;

				createTileMesh(Coordinate(ix, iz), name.str(), pCol);

				Ogre::Entity* zoneEntity = GameManager::getSingleton().getSceneManager()->createEntity("entity: " + name.str(), name.str());
				//testCity->setMaterialName("Test/ColourTest");
				zoneEntity->setMaterialName("Examples/Rockwall");
				thisSceneNode->attachObject(zoneEntity);
			}
		}
	}
}

/// creates a plane mesh using Ogre's Mesh manager
/// \param pName name of the plane (in format tile_x-z)
void LevelGenerator::createPlane(std::string pName)
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		pName,
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		scalar, scalar, 2, 2,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);
}


void LevelGenerator::createTileMesh(Coordinate pPosition, std::string pName, Ogre::ColourValue pCol) const {
	//TODO: add uv coordinates

	Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual(pName, "General");

	Ogre::SubMesh* sub = mesh->createSubMesh();

	const float sqrt13(1 / 3);
	int x = pPosition.x * scalar;
	int y = 0;
	int z = pPosition.z * scalar;

	//create vertices
	const size_t nVertices = 4;
	const size_t vBufCount = 8 * nVertices;
	float vertices[vBufCount] = {
		static_cast<float>(1000), static_cast<float>(0), static_cast<float>(0),			 //1
		sqrt13,  -sqrt13, -sqrt13,
		0,0,
		static_cast<float>(0), static_cast<float>(0), static_cast<float>(0),			 //2
		-sqrt13, -sqrt13, -sqrt13,
		0,1,
		static_cast<float>(1000), static_cast<float>(0), static_cast<float>(1000),		 //3
		sqrt13,  -sqrt13, sqrt13,
		1,0,
		static_cast<float>(0), static_cast<float>(0), static_cast<float>(1000),			 //4
		-sqrt13, -sqrt13, sqrt13,
		1,1
	};

	Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
	Ogre::RGBA colours[nVertices];
	Ogre::RGBA* colorPtr = colours;

	//TODO: add colour per zone 
	Ogre::ColourValue col = Ogre::ColourValue(1.0f, 0.0f, 0.0f, 1.0f);

	rs->convertColourValue(pCol, colorPtr++); //0
	rs->convertColourValue(pCol, colorPtr++); //1
	rs->convertColourValue(pCol, colorPtr++); //2
	rs->convertColourValue(pCol, colorPtr); //3

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

	mesh->_setBounds(Ogre::AxisAlignedBox(x, y, z, x + scalar, y + scalar, z + scalar));

	mesh->load();
}