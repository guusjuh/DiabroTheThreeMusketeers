#ifndef LEVEL_MANAGER_H_
#define LEVEL_MANAGER_H_

#include "Player.h"
#include "Npc.h"
#include "BaseApplication.h"
#include "BasicEnemy.h"
#include <OgreSceneManager.h>
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

enum MaterialType
{
	NpcMT,
	HousesMT
};

/// <summary>
/// The manager of the level. 
/// </summary>
class LevelManager {
public:
	LevelManager();
	~LevelManager() {}

	void initialize();
	void inGameUpdate(const Ogre::FrameEvent&);

	void nextFloor();
	void restartGame();
	void generateNewDungeon();

	int getCurrentLevel() { return _level; }
	LevelGenerator* getLevelGenerator() { return _levelGenerator; }

	Ogre::SceneNode* getCamNode() { return _camNode; }
	Ogre::Degree startPitchCam;

	Ogre::SceneNode* getLevelNode() { return _levelNode; }
	static void createGroundMesh();
	static void createNPCMesh();
	static void createEnemyMesh();
	static void createPlayerMesh();

	Player* getPlayer() { return playerScript; }
	std::vector<Character*> getFriendlyNpcs() { return _friendlyNpcScripts; }
	std::vector<Character*> getHostileNpcs() { return _hostileNpcScripts; }

	int subscribeHostileNPC(BasicEnemy*);
	int subscribeFriendlyNPC(Npc*);
	void detachHostileNPC(int);
	void detachFriendlyNPC(int);

	void spawnEnemy(City *pCity, bool pInstant);

private:
	int _level;

	std::vector<std::pair<Timer, City*>> enemySpawnTimers;

	Player* playerScript;

	std::vector<Character*> _friendlyNpcScripts;
	std::vector<Character*> _hostileNpcScripts;

	Ogre::SceneNode* _levelNode;
	Ogre::SceneNode* _camNode;
	LevelGenerator* _levelGenerator;
};

#endif
