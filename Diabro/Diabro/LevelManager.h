#pragma once
#include "Player.h"

class LevelManager
{
public:
	LevelManager(Ogre::Camera*, Ogre::SceneManager*);

	void Init();

	void Update(const Ogre::FrameEvent& fe);

	static void CreateGroundMesh();
	static void CreatePlane(std::string name);

	Ogre::Vector3 GetPlayerDirection();
	void SetPlayerDirection();

	// for now public so that game manager can access it. TO DO: implement singleton for GM.
	Player* _playerScript;
	Ogre::SceneNode* _playerNode;
	Ogre::Degree _startPitchCam;

private:
	Ogre::SceneManager* _sceneManager;
	Ogre::SceneNode* _levelNode;
	//playernode
	Ogre::SceneNode* _camNode;

	Ogre::Camera* _camera;
	
	Ogre::Entity* _playerEntity;
};
