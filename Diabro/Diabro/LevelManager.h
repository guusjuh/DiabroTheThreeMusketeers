#ifndef LEVEL_MANAGER_H_
#define LEVEL_MANAGER_H_

#include "Player.h"
#include "Npc.h"
#include "Sister.h"
#include "BaseApplication.h"
#include "BaseEnemy.h"
#include <OgreSceneManager.h>
#include "LevelGenerator.h"
#include "Bullet.h"
#include "Particle.h"

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
	Sister* getSister() { return _sisterScript; }
	std::vector<Character*> getFriendlyNpcs() { return _friendlyNpcScripts; }
	std::vector<Character*> getHostileNpcs() { return _hostileNpcScripts; }

	int subscribeHostileNPC(BaseEnemy*);
	int subscribeFriendlyNPC(Npc*);
	int subscribeBullet(Bullet*);
	int subscribeParticle(Particle*);
	void subscribeSister(Sister*);
	void detachHostileNPC(int);
	void detachFriendlyNPC(int);
	void detachBullet(int);
	void detachParticle(int);

	void spawnEnemy(City *pCity, bool pInstant);

private:
	int _level;

	std::vector<std::pair<Timer, City*>> enemySpawnTimers;

	Player* playerScript;

	std::vector<Character*> _friendlyNpcScripts;
	std::vector<Character*> _hostileNpcScripts;
	std::vector<Bullet*> _bulletScripts;
	std::vector<Particle*> _particleScripts;
	Sister* _sisterScript;

	Ogre::SceneNode* _levelNode;
	Ogre::SceneNode* _camNode;
	LevelGenerator* _levelGenerator;
};

#endif
