#include "GameManager.h"
#include "LevelManager.h"
#include "Debug.h"


/// <summary>
/// Initializes a new instance of the <see cref="LevelManager" /> class.
/// This class is created by the <see cref="GameManager" /> and contains all level information
/// like characters and the environment.
/// </summary>
LevelManager::LevelManager() : _playerEntity(0), _npcEntity(0), _basicEnemyEntity(0), _groundEntity(0),
playerScript(0), _levelNode(0), _camNode(0), npcSpawner(0)
{

}

/// <summary>
/// Initializes this the level by setting the camera, player, NPC's and surroundings.
/// </summary>
void LevelManager::initialize()
{
	_level = 1;

	// create level node, the root node for everything in the level
	_levelNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("LevelNode");
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("LevelGen Start!\n");
	fclose(fp);
#endif
	levelGenerator = new LevelGenerator();

	Ogre::SceneNode* playerNode = _levelNode->createChildSceneNode("PlayerNode");
	_camNode = playerNode->createChildSceneNode("CameraNode");

	//player
	_playerEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	playerNode->createChildSceneNode()->attachObject(_playerEntity);
	playerNode->setPosition(levelGenerator->getStartPos());
	playerNode->setScale(0.75f, 0.75f, 0.75f);
	playerScript = new Player(playerNode, _playerEntity);

	// camera
	_camNode->attachObject(GameManager::getSingletonPtr()->getCamera());
	_camNode->setPosition(Ogre::Vector3(0, 100, 0));
	_camNode->pitch(Ogre::Degree(15), Ogre::Node::TS_LOCAL);
	startPitchCam = _camNode->getOrientation().getPitch();
}

/// <summary>
/// Resets the level.
/// </summary>
void LevelManager::reset() {
	_level++;

	while(_friendlyNpcScripts.size() > 0) {
		detachFriendlyNPC(0);
	}
	while (_hostileNpcScripts.size() > 0) {
		detachHostileNPC(0);
	}

	// create level node, the root node for everything in the level
	_levelNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("LevelNode");
	Debug("restarting levelGenerator");
	levelGenerator->restart();

	Ogre::SceneNode* playerNode = _levelNode->createChildSceneNode("PlayerNode");
	_camNode = playerNode->createChildSceneNode("CameraNode");

	//player
	Debug("spawning player");
	_playerEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	playerNode->createChildSceneNode()->attachObject(_playerEntity);
	playerNode->setPosition(levelGenerator->getStartPos());
	playerScript->reset(playerNode, _playerEntity);

	// camera
	_camNode->attachObject(GameManager::getSingletonPtr()->getCamera());
	_camNode->setPosition(Ogre::Vector3(0, 100, 0));
	_camNode->pitch(Ogre::Degree(15), Ogre::Node::TS_LOCAL);
	startPitchCam = _camNode->getOrientation().getPitch();
}

/// <summary>
/// Subscribes the friendly NPC.
/// </summary>
/// <param name="friendly">The friendly.</param>
/// <returns></returns>
int LevelManager::subscribeFriendlyNPC(Npc* friendly) {
	_friendlyNpcScripts.push_back(friendly); 

	return _friendlyNpcScripts.size() - 1;
}

/// <summary>
/// Subscribes the hostile NPC.
/// </summary>
/// <param name="hostile">The hostile.</param>
/// <returns></returns>
int LevelManager::subscribeHostileNPC(BasicEnemy* hostile) {
	_hostileNpcScripts.push_back(hostile);

	return _hostileNpcScripts.size() - 1;
}

/// <summary>
/// Detaches the friendly NPC.
/// </summary>
/// <param name="id">The identifier.</param>
void LevelManager::detachFriendlyNPC(int id) {	
	_friendlyNpcScripts.erase(_friendlyNpcScripts.begin() + id);
	//reset id values
	for (std::vector<Character*>::iterator it = _friendlyNpcScripts.begin() + id; it < _friendlyNpcScripts.end(); ++it) {
		(*it)->id -= 1;
	}
}

/// <summary>
/// Detaches the hostile NPC.
/// </summary>
/// <param name="id">The identifier.</param>
void LevelManager::detachHostileNPC(int id) {
	_hostileNpcScripts.erase(_hostileNpcScripts.begin() + id);
	//reset id values
	for (std::vector<Character*>::iterator it = _hostileNpcScripts.begin() + id; it < _hostileNpcScripts.end(); ++it) {
		(*it)->id -= 1;
	}
}


/// <summary>
/// Updates the frame based on the specified fe.
/// </summary>
/// <param name="fe">The frame event.</param>
void LevelManager::inGameUpdate(const Ogre::FrameEvent& pFE)
{
	Ogre::Real deltaTime = pFE.timeSinceLastFrame;
	if (deltaTime > 0.1f){
		deltaTime = 0.1f;
	}
	// update characters
	playerScript->update(deltaTime);

	for(int i = 0; i < _friendlyNpcScripts.size(); i++)
	{
		_friendlyNpcScripts[i]->update(deltaTime);
	}

	for (int i = 0; i < _hostileNpcScripts.size(); i++)
	{
		_hostileNpcScripts[i]->update(deltaTime);
	}

	for (int i = 0; i < levelGenerator->getZone(0, 0).cities.size(); ++i) {
		levelGenerator->getZone(0, 0).cities[i].update();
	}
}

/// <summary>
/// Creates the ground mesh.
/// </summary>
void LevelManager::createGroundMesh()
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane(
		"ground",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane,
		100000, 100000, 20, 20,
		true,
		1, 200, 200,
		Ogre::Vector3::UNIT_Z);

	return;
}
