#include "GameManager.h"
#include "LevelManager.h"

LevelManager::LevelManager() : _playerEntity(0), _npcEntity(0), _basicEnemyEntity(0), _groundEntity(0)
{
}

/// <summary>
/// Initializes this instance.
/// </summary>
void LevelManager::Init()
{
	// create level node, the root node for everything in the level
	_levelNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("LevelNode");

	Ogre::SceneNode* playerNode = _levelNode->createChildSceneNode("PlayerNode");
	_camNode = playerNode->createChildSceneNode("CameraNode");

	// create player
	_playerEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("ninja.mesh");
	playerNode->createChildSceneNode()->attachObject(_playerEntity);
	playerScript = new Player(playerNode, _playerEntity);
	playerScript->initialize();

	//creating a NPC object
	Ogre::SceneNode* npcNode = _levelNode->createChildSceneNode("NpcNode");
	_npcEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("penguin.mesh");
	npcNode->createChildSceneNode()->attachObject(_npcEntity);
	npcScript = new Npc(npcNode, _npcEntity);
	npcScript->initialize();

	// create enemy
	Ogre::SceneNode* enemyNode = _levelNode->createChildSceneNode("EnemyNode");
	_basicEnemyEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("robot.mesh");
	enemyNode->createChildSceneNode()->attachObject(_basicEnemyEntity);
	enemyScript = new BasicEnemy(enemyNode, _basicEnemyEntity);
	enemyScript->initialize();

	// ground 
	createGroundMesh();
	_groundEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("ground");
	_levelNode->createChildSceneNode()->attachObject(_groundEntity);
	_groundEntity->setMaterialName("Examples/Rockwall");

	// camera
	_camNode->attachObject(GameManager::getSingletonPtr()->getCamera());
	_camNode->pitch(Ogre::Degree(10), Ogre::Node::TS_LOCAL);
	startPitchCam = _camNode->getOrientation().getPitch();

	_npcScripts.push_back(npcScript);
	_npcScripts.push_back(enemyScript);
}

void LevelManager::Update(const Ogre::FrameEvent& fe)
{
	// Update characters
	playerScript->update(fe.timeSinceLastFrame);

	for(int i = 0; i < _npcScripts.size(); i++)
	{
		_npcScripts[i]->update(fe.timeSinceLastFrame);
	}
}

void LevelManager::createGroundMesh()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		1500, 1500, 20, 20,
		true,
		1, 5, 5,
		Ogre::Vector3::UNIT_Z);

	return;
}