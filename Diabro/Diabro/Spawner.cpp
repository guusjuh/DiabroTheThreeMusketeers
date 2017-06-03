#include "Spawner.h"
#include "GameManager.h"
#include "BasicEnemy.h"

/// <summary>
/// Initializes a new instance of the <see cref="Spawner"/> class.
/// </summary>
/// <param name="pMyNode">The my node.</param>
/// <param name="pNumOfSpawns">The number of spawns.</param>
/// <param name="pSpawnPosition">The spawn position.</param>
/// <param name="pMyCity">The my city.</param>
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
	int randomX = rand() % 250 + 250; //(100 - -100 + 1) + -100;
	int randomZ = rand() % 250 + 250; //(100 - -100 + 1) + -100;
	instanceNode->translate(_spawnPosition + Ogre::Vector3(randomX, 0, randomZ), Ogre::Node::TS_WORLD);

	Ogre::Entity* instanceEntity = GameManager::getSingletonPtr()->getSceneManager()->createEntity(typeid(T) == typeid(Npc) ? "uv_sphere.mesh" : "cube.mesh");
	//rotateNode
	Ogre::SceneNode* rotationNode = instanceNode->createChildSceneNode();
		
	rotationNode->attachObject(instanceEntity);
	T* instanceScript = new T(instanceNode, rotationNode, instanceEntity, _myCity, GameManager::getSingletonPtr()->getLevelManager()->getCurrentLevel());
}

template class Spawner < BasicEnemy >;
