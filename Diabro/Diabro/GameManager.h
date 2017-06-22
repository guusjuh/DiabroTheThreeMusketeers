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
#include "SoundManager.h"
#include "DialogManager.h"

//---------------------------------------------------------------------------

/// <summary>
/// Enum for the game states. 
/// </summary>
enum GameState {
	MainMenu = 0,
	Paused,
	Start, 
	InGame,
	End,
	Died
};

/// <summary>
/// The manager of the game.
/// </summary>
/// <seealso cref="BaseApplication" />
/// <seealso cref="Ogre::Singleton{GameManager}" />
class GameManager : public BaseApplication, public Ogre::Singleton<GameManager>
{
public:
	GameManager(void);
	~GameManager(void);

	Ogre::String nodeOfSceneXML;

	static GameManager& getSingleton(void);
	static GameManager* getSingletonPtr(void);

	GameState getCurrentState() { return currentState; }

	void goToState(GameState pState) {
		//check for player died before going to ingame so you can reset stuff
		if(currentState == Died && pState == InGame) {
			restartGame();
		}
		if (currentState == End) {
			nextFloor();
		}
		if(pState == Paused) {
			_uiManager->showPauseScreen();
		}
		else if(currentState == Paused) {
			_uiManager->hidePauseScreen();
		}

		if (pState == MainMenu) {
			_uiManager->showMainMenu();
		}
		else if (currentState == MainMenu) {
			_uiManager->hideMainMenu();
		}

		previousState = currentState;
		currentState = pState;
	}

	Ogre::SceneManager* getSceneManager(void) { return mSceneMgr; }
	Ogre::Camera* getCamera(void) { return mCamera; }
	Ogre::Timer* getGameTime(void) { return _gameTimer; }

	LevelManager* getLevelManager(void) { return _levelManager; }
	UIManager* getUIManager(void) { return _uiManager; }
	QuestManager* getQuestManager(void) { return _questManager; }
	SoundManager* getSoundManager(void) { return _soundManager; }
	DialogManager* getDialogManager(void) { return _dialogManager; }
	Player* getPlayer() { return getLevelManager()->getPlayer(); }

	int getRandomInRange(int pLO, int pHI) {
		if (pHI == 0) return 0;
		return rand() % (pHI-pLO) + pLO;
	}

	void restartGame();
	bool up, down, left, right;
	bool fly, fall;

	float getDeltaTime(){ return _deltaTime; }

protected:
    virtual void createScene(void);
	virtual void createCamera(void);
	virtual void createViewports(void);
	virtual void createFrameListener(void);
	
	static void setupLights(Ogre::SceneManager*);
	
private:
	float _deltaTime;

	virtual bool frameRenderingQueued(const Ogre::FrameEvent&);
	virtual bool keyPressed(const OIS::KeyEvent&);
	virtual bool keyReleased(const OIS::KeyEvent&);
	virtual bool mouseMoved(const OIS::MouseEvent&);
	virtual bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID);
	virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);
	
	void nextFloor();

	LevelManager* _levelManager;
	UIManager* _uiManager;
	QuestManager* _questManager;
	SoundManager* _soundManager;
	DialogManager* _dialogManager;

	Ogre::Timer* _gameTimer;

	GameState currentState;
	GameState previousState;

	bool _abandonedQuestPressed;
	float _abandonTimer;
	const float _totalTimeBeforeAbandon;
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameManager_h_

//---------------------------------------------------------------------------
