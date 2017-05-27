#ifndef LEVEL_MANAGER_H_
#define LEVEL_MANAGER_H_

#include "Player.h"
#include "Npc.h"
#include "BaseApplication.h"
#include "BasicEnemy.h"
#include <OgreSceneManager.h>
#include "Spawner.h"
#include "LevelGenerator.h"

/// <summary>
/// The manager of the level. 
/// </summary>
class LevelManager
{
public:
	LevelManager();

	void initialize();
	void reset();

	void inGameUpdate(const Ogre::FrameEvent&);

	static void createGroundMesh();

	Ogre::SceneNode* getLevelNode() { return _levelNode; }
	Ogre::SceneNode* getCamNode() { return _camNode; }

	Player* getPlayer() { return playerScript; }
	std::vector<Character*> getFriendlyNpcs() { return _friendlyNpcScripts; }
	std::vector<Character*> getHostileNpcs() { return _hostileNpcScripts; }

	int subscribeHostileNPC(BasicEnemy*);
	int subscribeFriendlyNPC(Npc*);

	void detachHostileNPC(int);
	void detachFriendlyNPC(int);
	static int testunit(int i) { return ++i; };
	static int testunittwo(int i);
	
	LevelGenerator* levelGenerator;
	// for now public so that game manager can access it. 
	Player* playerScript; 

	Ogre::Degree startPitchCam;
	Ogre::Vector3 playerPosition;
	std::string npcNames[8];

private:
	Ogre::Entity* _playerEntity;
	//TODO replace all with spawners
	Ogre::Entity* _npcEntity;
	Ogre::Entity* _basicEnemyEntity;

	Spawner<Npc>* npcSpawner;
	Spawner<BasicEnemy>* enemySpawner;

	Ogre::Entity* _groundEntity;

	Ogre::SceneNode* _levelNode;
	Ogre::SceneNode* _camNode;
	
	std::vector<Character*> _friendlyNpcScripts;
	std::vector<Character*> _hostileNpcScripts;
	// TODO: lists of different scripts (NPC's, enemies, e.d.) 
};

#endif
