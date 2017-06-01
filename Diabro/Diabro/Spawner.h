#ifndef CHARACTER_SPAWNER_H_
#define CHARACTER_SPAWNER_H_

#include "Character.h"

/// <summary>
/// A generic spawner class.
/// </summary>
template <class T>
class Spawner {
public:
	Spawner<T>(Ogre::SceneNode*, int, Ogre::Vector3, City*);
	~Spawner();

	void instanceDeath();
private:
	void spawnInstance();

	Ogre::SceneNode* _myNode;
	Ogre::Vector3 _spawnPosition;

	//amount of characters to spawn
	int _nInstances;

	City* _myCity;
};
#endif