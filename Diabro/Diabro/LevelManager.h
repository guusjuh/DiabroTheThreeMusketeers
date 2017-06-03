#ifndef LEVEL_MANAGER_H_
#define LEVEL_MANAGER_H_

#include "Player.h"
#include "Npc.h"
#include "BaseApplication.h"
#include "BasicEnemy.h"
#include <OgreSceneManager.h>
#include "Spawner.h"
#include "LevelGenerator.h"

struct Timer {
public:
	float timeLeft;
	bool isFinished;

	Timer(float time) : isFinished(false), timeLeft(time){ }
	void update(float deltaTime) {
		timeLeft -= deltaTime;
		if (timeLeft <= 0.0f) {
			isFinished = true;
		}
	}
};

/// <summary>
/// The manager of the level. 
/// </summary>
class LevelManager
{
public:
	LevelManager();

	void initialize();
	void nextFloor();
	void restartGame();
	void generateNewDungeon();

	int getCurrentLevel() { return _level; }

	void inGameUpdate(const Ogre::FrameEvent&);

	static void createGroundMesh();
	static void createNPCMesh();
	static void createEnemyMesh();
	static void createPlayerMesh();

	Ogre::SceneNode* getLevelNode() { return _levelNode; }
	Ogre::SceneNode* getCamNode() { return _camNode; }

	void spawnEnemy(City *pCity, float timer);

	Player* getPlayer() { return playerScript; }
	std::vector<Character*> getFriendlyNpcs() { return _friendlyNpcScripts; }
	std::vector<Character*> getHostileNpcs() { return _hostileNpcScripts; }

	int subscribeHostileNPC(BasicEnemy*);
	int subscribeFriendlyNPC(Npc*);

	void detachHostileNPC(int);
	void detachFriendlyNPC(int);
	
	LevelGenerator* levelGenerator;
	// for now public so that game manager can access it. 
	Player* playerScript; 

	Ogre::Degree startPitchCam;
	Ogre::Vector3 playerPosition;
	std::string npcNames[8];

private:
	std::vector<std::pair<Timer, City*>> enemySpawnTimers;

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

	int _level;
};

#endif
