#include "Spawner.h"
#include "GameManager.h"

template<class T>
::Spawner<T>::Spawner(Ogre::SceneNode* pMyNode, int pNumOfSpawns, Ogre::Vector3 pSpawnPosition, City* pMyCity)
	: _myNode(pMyNode), _spawnPosition(pSpawnPosition), _nInstances(pNumOfSpawns), _myCity(pMyCity){
	for (int i = 0; i < _nInstances; ++i) {
		spawnInstance();
	}
}

/// <summary>
/// Finalizes an instance of the <see cref="Spawner"/> class.
/// </summary>
template<class T>
Spawner<T>::~Spawner() {

}

/// <summary>
/// called when an <T> instance dies (does not kill instance).
/// </summary>
template<class T>
void Spawner<T>::instanceDeath() {
	spawnInstance();
}

/// <summary>
/// Spawns an instance of <T>.
/// </summary>
template<class T>
void Spawner<T>::spawnInstance() {
	
	Ogre::SceneNode* instanceNode = _myNode->createChildSceneNode();
	//random offset
	int randomX = rand() % (100 - -100 + 1) + -100;
	int randomZ = rand() % (100 - -100 + 1) + -100;
	instanceNode->translate(_spawnPosition + Ogre::Vector3(randomX, 0, randomZ), Ogre::Node::TS_WORLD);

	Ogre::Entity* instanceEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity(typeid(T) == typeid(Npc) ? "penguin.mesh" : "robot.mesh");
	//rotateNode
	Ogre::SceneNode* rotationNode = instanceNode->createChildSceneNode();
		
	rotationNode->attachObject(instanceEntity);
	T* instanceScript = new T(instanceNode, rotationNode, instanceEntity, _myCity);
}

template class Spawner < BasicEnemy >;