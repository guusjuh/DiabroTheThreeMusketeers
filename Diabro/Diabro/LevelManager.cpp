#include "GameManager.h"
#include "LevelManager.h"
#include "Debug.h"


/// <summary>
/// Initializes a new instance of the <see cref="LevelManager" /> class.
/// This class is created by the <see cref="GameManager" /> and contains all level information
/// like characters and the environment.
/// </summary>
LevelManager::LevelManager() : playerScript(0), _levelNode(0), _camNode(0), 
	_friendlyNpcScripts(), _hostileNpcScripts(), _bulletScripts(), _particleScripts() { }

/// <summary>
/// Initializes this the level by setting the camera, player, NPC's and surroundings.
/// </summary>
void LevelManager::initialize()
{
	_level = 1;

	// create level node, the root node for everything in the level
	_levelNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("LevelNode");
	_levelGenerator = new LevelGenerator();

	Ogre::SceneNode* playerNode = _levelNode->createChildSceneNode("PlayerNode");
	_camNode = playerNode->createChildSceneNode("CameraNode");

	//player
	Ogre::Entity* playerEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	playerNode->createChildSceneNode()->attachObject(playerEntity);
	playerNode->setPosition(_levelGenerator->getStartPos());
	playerNode->setScale(0.75f, 0.75f, 0.75f);
	playerScript = new Player(playerNode, playerEntity);

	_levelGenerator->initialize();

	// camera
	_camNode->attachObject(GameManager::getSingletonPtr()->getCamera());
	_camNode->setPosition(Ogre::Vector3(0, 100, -200));
	_camNode->pitch(Ogre::Degree(15), Ogre::Node::TS_LOCAL);
	startPitchCam = _camNode->getOrientation().getPitch();
}

/// <summary>
/// Resets the level.
/// </summary>
void LevelManager::nextFloor() {
	Debug("\tL: spawning next floor, player succeeded.");
	_level++;
	GameManager::getSingletonPtr()->getUIManager()->increaseFloorText();
	enemySpawnTimers.clear();

	generateNewDungeon();
}

/// <summary>
/// Resets the level.
/// </summary>
void LevelManager::restartGame() {
	Debug("\tL: restarting the game, player died.");
	_level = 1;
	enemySpawnTimers.clear();

	generateNewDungeon();
}

/// <summary>
/// Resets the level.
/// </summary>
void LevelManager::generateNewDungeon() {
	Debug("\t\tTHE CURRENT LEVEL IS ", _level);

	Debug("\tL: generating new dungeon.");
	while (_friendlyNpcScripts.size() > 0) {
		delete _friendlyNpcScripts[0];
		detachFriendlyNPC(0);
	}
	while (_hostileNpcScripts.size() > 0) {
		delete _hostileNpcScripts[0];
		detachHostileNPC(0);
	}
	while (_bulletScripts.size() > 0) {
		delete _bulletScripts[0];
		detachBullet(0);
	}
	while (_particleScripts.size() > 0) {
		delete _particleScripts[0];
		detachParticle(0);
	}

	// create level node, the root node for everything in the level
	_levelNode = GameManager::getSingletonPtr()->getSceneManager()->getRootSceneNode()->createChildSceneNode("LevelNode");
	Debug("restarting _levelGenerator");
	_levelGenerator->restart();

	Ogre::SceneNode* playerNode = _levelNode->createChildSceneNode("PlayerNode");
	_camNode = playerNode->createChildSceneNode("CameraNode");

	//player
	Debug("spawning player");
	Ogre::Entity* playerEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity("uv_sphere.mesh");
	playerNode->createChildSceneNode()->attachObject(playerEntity);
	playerNode->setPosition(_levelGenerator->getStartPos());
	playerScript->reset(playerNode, playerEntity);

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
int LevelManager::subscribeHostileNPC(BaseEnemy* hostile) {
	_hostileNpcScripts.push_back(hostile);

	return _hostileNpcScripts.size() - 1;
}

/// <summary>
/// Subscribes a bullet.
/// </summary>
/// <param name="hostile">The bullet.</param>
/// <returns></returns>
int LevelManager::subscribeBullet(Bullet* bullet) {
	_bulletScripts.push_back(bullet);

	return _bulletScripts.size() - 1;
}

/// <summary>
/// Subscribes a particle.
/// </summary>
/// <param name="hostile">The bullet.</param>
/// <returns></returns>
int LevelManager::subscribeParticle(Particle* particle) {
	_particleScripts.push_back(particle);

	return _particleScripts.size() - 1;
}

void LevelManager::spawnEnemy(City *pCity, bool pInstant) {
	if (pCity != nullptr) {
		if (pInstant) {
			_levelGenerator->spawnEnemy(pCity, 1);
		}
		else {
			enemySpawnTimers.push_back(std::make_pair(Timer(pCity->getEnemyRespawnTimeForUsage()), pCity));
		}
	}
}

/// <summary>
/// Detaches the friendly NPC.
/// </summary>
/// <param name="id">The identifier.</param>
void LevelManager::detachFriendlyNPC(int id) {	
	_friendlyNpcScripts.erase(_friendlyNpcScripts.begin() + id);
	//nextFloor id values
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
	//nextFloor id values
	for (std::vector<Character*>::iterator it = _hostileNpcScripts.begin() + id; it < _hostileNpcScripts.end(); ++it) {
		(*it)->id -= 1;
	}
}

/// <summary>
/// Detaches the bullet.
/// </summary>
/// <param name="id">The identifier.</param>
void LevelManager::detachBullet(int id) {
	_bulletScripts.erase(_bulletScripts.begin() + id);
	//nextFloor id values
	for (std::vector<Bullet*>::iterator it = _bulletScripts.begin() + id; it < _bulletScripts.end(); ++it) {
		(*it)->id -= 1;
	}
}

/// <summary>
/// Detaches the hostile NPC.
/// </summary>
/// <param name="id">The identifier.</param>
void LevelManager::detachParticle(int id) {
	_particleScripts.erase(_particleScripts.begin() + id);
	//nextFloor id values
	for (std::vector<Particle*>::iterator it = _particleScripts.begin() + id; it < _particleScripts.end(); ++it) {
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

	for (int i = 0; i < _bulletScripts.size(); i++)
	{
		_bulletScripts[i]->update(deltaTime);
	}

	for (int i = 0; i < _particleScripts.size(); i++)
	{
		_particleScripts[i]->update(deltaTime);
	}

	for (int i = 0; i < _levelGenerator->getZone(0, 0).cities.size(); ++i) {
		_levelGenerator->getZone(0, 0).cities[i].update();
	}
	if (enemySpawnTimers.size() > 0) {
		for (int i = 0; i < enemySpawnTimers.size(); i++) {
			if (enemySpawnTimers[i].first.isFinished) {
				spawnEnemy(enemySpawnTimers[i].second, true);
				enemySpawnTimers.erase(enemySpawnTimers.begin() + i);
				i--;
			} else {
				enemySpawnTimers[i].first.update(pFE.timeSinceLastFrame);
			}
		}
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
