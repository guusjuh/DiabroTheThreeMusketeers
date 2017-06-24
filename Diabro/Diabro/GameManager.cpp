/*
-----------------------------------------------------------------------------
Filename:    GameManager.cpp
-----------------------------------------------------------------------------

*/
#include "GameManager.h"
#include "SdkTrays.h"

//---------------------------------------------------------------------------

/// <summary>
/// Initializes a new instance of the <see cref="GameManager"/> class.
/// This class is the central manager of the game and has therefore the only singleton instance.
/// It contains all other managers.
/// </summary>
GameManager::GameManager() : _levelManager(0), _uiManager(0), _gameTimer(0), _questManager(0), _dialogManager(0),
up(false), down(false), left(false), right(false), fly(false), fall(false), _abandonedQuestPressed(false), _totalTimeBeforeAbandon(1.5f), _exitGamePressed(false), _totalTimeBeforeExit(1.5f) {}
//---------------------------------------------------------------------------
/// <summary>
/// Finalizes an instance of the <see cref="GameManager"/> class.
/// </summary>
GameManager::~GameManager()
{
	delete _gameTimer;
	delete _levelManager;
	delete _uiManager;
	delete _questManager;
	delete _dialogManager;
}

//---------------------------------------------------------------------------

template<> GameManager* Ogre::Singleton<GameManager>::msSingleton = 0;
/// <summary>
/// Gets the singleton pointer.
/// </summary>
/// <returns></returns>
GameManager* GameManager::getSingletonPtr(void)
{
	return msSingleton;
}

/// <summary>
/// Gets the singleton.
/// </summary>
/// <returns></returns>
GameManager& GameManager::getSingleton(void)
{
	assert(msSingleton);  return (*msSingleton);
}

//---------------------------------------------------------------------------

/// <summary>
/// Creates the scene.
/// </summary>
void GameManager::createScene(void)
{
	_gameTimer = new Ogre::Timer();

	_dialogManager = new DialogManager();

	_levelManager = new LevelManager();
	_levelManager->initialize();

	_uiManager = new UIManager();
	_uiManager->init();

	_soundManager = new SoundManager();

	_questManager = new QuestManager();
	_questManager->Initialize();

	// set lights
	setupLights(mSceneMgr);

	// set shadow technique
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	currentState = (GameState)0;
	previousState = currentState;
	goToState(MainMenu);
}

/// <summary>
/// Resets the game.
/// </summary>
void GameManager::nextFloor() {
	Debug("\tG: spawning next floor, player succeeded.");
	getSceneManager()->clearScene();

	_levelManager->nextFloor();
	_questManager->reset();

	setupLights(mSceneMgr);

	previousState = currentState;
	currentState = Start;

	up = false;
	down = false;
	left = false;
	right = false;
}

/// <summary>
/// Resets the game.
/// </summary>
void GameManager::restartGame() {
	Debug("\tG: restarting the game, player died.");
	getSceneManager()->clearScene();
	
	_levelManager->restartGame();
	_questManager->reset();

	setupLights(mSceneMgr);

	previousState = currentState;
	currentState = Start;

	up = false;
	down = false;
	left = false;
	right = false;
}

/// <summary>
/// Setups the lights.
/// </summary>
/// <param name="pSceneMgr">The scenemanager.</param>
void GameManager::setupLights(Ogre::SceneManager* pSceneMgr)
{
	// set ambient lighting
	pSceneMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));

	// create the main light
	Ogre::Light* light = pSceneMgr->createLight("MainLight");
	light->setDiffuseColour(0.4, 0.4, 0.4);
	light->setSpecularColour(0.3, 0.3, 0.3);
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(-1, -2, -2);
	return;
}

/// <summary>
/// Creates the camera.
/// </summary>
void GameManager::createCamera()
{
	// create the camera
	mCamera = mSceneMgr->createCamera("MainCam");

	// set pos and rot
	mCamera->setPosition(Ogre::Vector3(0, 200, 300));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setNearClipDistance(5);

	// create the camera man
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}

/// <summary>
/// Creates the viewports.
/// </summary>
void GameManager::createViewports()
{
	// add a viewport
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);

	// set background viewport
	vp->setBackgroundColour(Ogre::ColourValue(0.278f, 0.368f, 0.482f));
	
	// as aspect ratio to avoid distortion
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
}

/// <summary>
/// Creates the frame listener.
/// </summary>
void GameManager::createFrameListener(void)
{
	BaseApplication::createFrameListener();

	return;
}

/// <summary>
/// Updates the frame based on the specified pFE.
/// </summary>
/// <param name="pFE">The frame event.</param>
bool GameManager::frameRenderingQueued(const Ogre::FrameEvent& pFE)
{
	bool ret = BaseApplication::frameRenderingQueued(pFE);

	_deltaTime = pFE.timeSinceLastFrame;

	switch (currentState) {
	case Start:
		_uiManager->startUpdate(pFE);
		break;
	case Died:
		_uiManager->diedUpdate(pFE);
		break;
	case InGame:
		if (getPlayer()->isInDialog()) return ret;

		if (_questManager->getCurrentQuest() != nullptr) {
			_questManager->getCurrentQuest()->update();
		}
		_levelManager->inGameUpdate(pFE);
		_uiManager->inGameUpdate(pFE);
		break;
	case End:
		_uiManager->endUpdate(pFE);
		break;
	}

	if(_abandonedQuestPressed) {
		_abandonTimer -= pFE.timeSinceLastFrame;
		if(_abandonTimer < 0) {
			_abandonedQuestPressed = false;
		}
	}

	if (_exitGamePressed) {
		_exitTimer -= pFE.timeSinceLastFrame;
		if (_exitTimer < 0) {
			_exitGamePressed = false;
		}
	}

	return ret;
}

/// <summary>
/// Called when a key is pressed.
/// </summary>
/// <param name="pKE">The key event.</param>
/// <returns></returns>
bool GameManager::keyPressed(const OIS::KeyEvent& pKE)
{
	if (pKE.key == OIS::KC_ESCAPE && currentState != Paused) {
		if (_exitGamePressed) {
			mShutDown = true;
		}
		else {
			_uiManager->showHUDText("Press 'ESC' again to exit the game.", 1.5f);
			_exitGamePressed = true;
			_exitTimer = _totalTimeBeforeExit;
		}
	}

	if (currentState != InGame && currentState != Paused && currentState != MainMenu) return false;

	if(currentState == MainMenu) {
		goToState(Start);
		return true;
	}

	Ogre::Vector3 dirVec = getPlayer()->getDirVector();

	switch (pKE.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
		up = true;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		down = true;
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		left = true;
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		right = true;
		break;
	case OIS::KC_F1:
		fly = true;
		break;
	case OIS::KC_F2:
		fall = true;
		break;
		

	case OIS::KC_BACK:
		if(_questManager->getCurrentQuest()) {
			if(_abandonedQuestPressed) {
				_questManager->getCurrentQuest()->abandon();
			}
			else {
				_uiManager->showHUDText("Press 'BACKSPACE' again to abandon your current quest.", 1.5f);
				_abandonedQuestPressed = true;
				_abandonTimer = _totalTimeBeforeAbandon;
			}
		}
		break;

	case OIS::KC_E:
		getPlayer()->interactionTriggered();
		break;

	case OIS::KC_LSHIFT:
		getPlayer()->setRunning(true);
		break;

	case OIS::KC_SPACE:
		if(getPlayer()->isInDialog()) {
			getPlayer()->dialogTriggered();
		}
		break;

	case OIS::KC_P:
		if(currentState == Paused) {
			goToState(previousState);
		} else {
			goToState(Paused);
		}

	default:
		break;
	}

	getPlayer()->setDirVector(dirVec);
	return true;
}

/// <summary>
/// Called when a key is released.
/// </summary>
/// <param name="pKE">The key event.</param>
/// <returns></returns>
bool GameManager::keyReleased(const OIS::KeyEvent& pKE)
{
	if (currentState != InGame && currentState != Paused) return false;

	Ogre::Vector3 dirVec = getPlayer()->getDirVector();

	switch (pKE.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
		up = false;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		down = false;
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		left = false;
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		right = false;
		break;
	case OIS::KC_F1:
		fly = false;
		break;
	case OIS::KC_F2:
		fall = false;
		break;

	case OIS::KC_LSHIFT:
		getPlayer()->setRunning(false);
		break;

	default:
		break;
	}

	getPlayer()->setDirVector(dirVec);
	return true;
}

/// <summary>
/// Called when the mouse is moved.
/// </summary>
/// <param name="me">The mouse event.</param>
/// <returns></returns>
bool GameManager::mouseMoved(const OIS::MouseEvent& pME)
{
	if (currentState == Paused) return false;

	Ogre::Degree rotX = Ogre::Degree(-getPlayer()->getRotationspeed()/2 * pME.state.Y.rel);
	Ogre::Degree originalPitch = mSceneMgr->getSceneNode("CameraNode")->getOrientation().getPitch();
	Ogre::Degree degreeFrmStartPitch = (rotX + originalPitch) - _levelManager->startPitchCam;

	mSceneMgr->getSceneNode("PlayerNode")->yaw(Ogre::Degree(-getPlayer()->getRotationspeed() * pME.state.X.rel), Ogre::Node::TS_WORLD);

	if (degreeFrmStartPitch < Ogre::Degree(25) && degreeFrmStartPitch > Ogre::Degree(-30))
	{
		mSceneMgr->getSceneNode("CameraNode")->pitch(rotX, Ogre::Node::TS_LOCAL);
	}

	return true;
}

/// <summary>
/// Called when the mouse is pressed.
/// </summary>
/// <param name="me">The mouse event.</param>
/// <param name="id">The identifier of the mouse button.</param>
/// <returns></returns>
bool GameManager::mousePressed(const OIS::MouseEvent& pME, OIS::MouseButtonID pID)
{
	if (currentState == Paused) return false;

	switch (pID)
	{
	case OIS::MB_Left:
		getPlayer()->lightAttack();
		break;
	default:
		break;
	}

	return true;
}

/// <summary>
/// Called when the mouse is released.
/// </summary>
/// <param name="me">The mouse event.</param>
/// <param name="id">The identifier of the mouse button.</param>
/// <returns></returns>
bool GameManager::mouseReleased(const OIS::MouseEvent& pME, OIS::MouseButtonID pID)
{
	if (currentState == Paused) return false;

	return true;
}

//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		AllocConsole();
#endif

        // Create application object
        GameManager app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
		
        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
