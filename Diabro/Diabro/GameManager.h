/*
-----------------------------------------------------------------------------
Filename:    GameManager.h
-----------------------------------------------------------------------------
*/

#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

#include "BaseApplication.h"
#include "LevelManager.h"
#include <OgreSingleton.h>
#include "UIManager.h"
#include "QuestContentManager.h"
#include <ctime>
#include "QuestManager.h"

//---------------------------------------------------------------------------

enum GameState {
	Start = 0, 
	InGame,
	End
};

class GameManager : public BaseApplication, public Ogre::Singleton<GameManager>
{
public:
	GameManager(void);
	~GameManager(void);

	Ogre::String nodeOfSceneXML;

	static GameManager& getSingleton(void);
	static GameManager* getSingletonPtr(void);

	void goNextState() {
		if (state == End) reset();
		state = (GameState)(((int)state + 1) % 3);
	}

	Ogre::SceneManager* getSceneManager(void) { return mSceneMgr; }
	Ogre::Camera* getCamera(void) { return mCamera; }
	Ogre::Timer* getGameTime(void) { return _gameTimer; }

	LevelManager* getLevelManager(void) { return _levelManager; }
	UIManager* getUIManager(void) { return _uiManager; }
	QuestManager* getQuestManager(void) { return _questManager; }

	int getRandomInRange(int pLO, int pHI) {
		return rand() % pHI + pLO;
	}

protected:
    virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createFrameListener(void);
	
	static void setupLights(Ogre::SceneManager*);
	
private:
	virtual bool frameRenderingQueued(const Ogre::FrameEvent&);
	virtual bool keyPressed(const OIS::KeyEvent&);
	virtual bool keyReleased(const OIS::KeyEvent&);
	virtual bool mouseMoved(const OIS::MouseEvent&);
	virtual bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID);
	virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);

	void reset();

	LevelManager* _levelManager;
	UIManager* _uiManager;
	QuestManager* _questManager;

	Ogre::Timer* _gameTimer;

	GameState state;
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameManager_h_

//---------------------------------------------------------------------------
