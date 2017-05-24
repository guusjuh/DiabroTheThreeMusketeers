/*
-----------------------------------------------------------------------------
Filename:    GameManager.cpp
-----------------------------------------------------------------------------

*/
#include "GameManager.h"
#include "SdkTrays.h"
#include "Tree.h"

//---------------------------------------------------------------------------

/// <summary>
/// Initializes a new instance of the <see cref="GameManager"/> class.
/// This class is the central manager of the game and has therefore the only singleton instance.
/// It contains all other managers.
/// </summary>
GameManager::GameManager() : _levelManager(0), _uiManager(0), _gameTimer(0), _questManager(0)
{
}
//---------------------------------------------------------------------------
/// <summary>
/// Finalizes an instance of the <see cref="GameManager"/> class.
/// </summary>
GameManager::~GameManager()
{
	delete _gameTimer;
	delete _levelManager;
	delete _uiManager;
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

	_levelManager = new LevelManager();
	_levelManager->initialize();

	_uiManager = new UIManager();
	_uiManager->init();

	_questManager = new QuestManager();
	_questManager->Initialize();

	// set lights
	setupLights(mSceneMgr);

	// set shadow technique
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	state = Start;
	//_uiManager->startState();
}

void GameManager::reset() {
	getSceneManager()->clearScene();
	_levelManager->reset();

	state = Start;
}

/// <summary>
/// Setups the lights.
/// </summary>
/// <param name="pSceneMgr">The scenemanager.</param>
void GameManager::setupLights(Ogre::SceneManager* pSceneMgr)
{
	// set ambient lighting
	pSceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

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

	switch (state) {
	case Start:
		_uiManager->startUpdate(pFE);
		break;
	case Died:
		_uiManager->diedUpdate(pFE);
		break;
	case InGame:
		_levelManager->inGameUpdate(pFE);
		_uiManager->inGameUpdate(pFE);
		break;
	case End:
		_uiManager->endUpdate(pFE);
		break;
	}

	/*if (state == InGame) {
		_levelManager->inGameUpdate(pFE);
	}

	_uiManager->update(pFE);*/

	return ret;
}

/// <summary>
/// Called when a key is pressed.
/// </summary>
/// <param name="pKE">The key event.</param>
/// <returns></returns>
bool GameManager::keyPressed(const OIS::KeyEvent& pKE)
{
	if (pKE.key == OIS::KC_ESCAPE) mShutDown = true;

	if (state != InGame) return false;

	Ogre::Vector3 dirVec = _levelManager->playerScript->getDirVector();

	switch (pKE.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
		dirVec.z = -1;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		dirVec.z = 1;
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		dirVec.x = -1;
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		dirVec.x = 1;
		break;
		
	case OIS::KC_E:
		//TODO: find the closed NPC
		_levelManager->getPlayer()->dialogTriggered();
		break;

	case OIS::KC_SPACE:
		_levelManager->getPlayer()->dialogTriggered();
		break;

	default:
		break;
	}

	_levelManager->playerScript->setDirVector(dirVec);
	return true;
}

/// <summary>
/// Called when a key is released.
/// </summary>
/// <param name="pKE">The key event.</param>
/// <returns></returns>
bool GameManager::keyReleased(const OIS::KeyEvent& pKE)
{
	if (state != InGame) return false;

	Ogre::Vector3 dirVec = _levelManager->playerScript->getDirVector();

	switch (pKE.key)
	{
	case OIS::KC_UP:
	case OIS::KC_W:
		dirVec.z = 0;
		break;

	case OIS::KC_DOWN:
	case OIS::KC_S:
		dirVec.z = 0;
		break;

	case OIS::KC_LEFT:
	case OIS::KC_A:
		dirVec.x = 0;
		break;

	case OIS::KC_RIGHT:
	case OIS::KC_D:
		dirVec.x = 0;
		break;

	default:
		break;
	}

	_levelManager->playerScript->setDirVector(dirVec);
	return true;
}

//TODO: where/how should the turning be handled? 
/// <summary>
/// Called when the mouse is moved.
/// </summary>
/// <param name="me">The mouse event.</param>
/// <returns></returns>
bool GameManager::mouseMoved(const OIS::MouseEvent& pME)
{
	Ogre::Degree rotX = Ogre::Degree(-_levelManager->playerScript->getRotationspeed()/2 * pME.state.Y.rel);
	Ogre::Degree originalPitch = mSceneMgr->getSceneNode("CameraNode")->getOrientation().getPitch();
	Ogre::Degree degreeFrmStartPitch = (rotX + originalPitch) - _levelManager->startPitchCam;

	mSceneMgr->getSceneNode("PlayerNode")->yaw(Ogre::Degree(-_levelManager->playerScript->getRotationspeed() * pME.state.X.rel), Ogre::Node::TS_WORLD);

	if (degreeFrmStartPitch < Ogre::Degree(10) && degreeFrmStartPitch > Ogre::Degree(-40))
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
	switch (pID)
	{
	case OIS::MB_Left:
		_levelManager->playerScript->lightAttack();
		break;
	default:
		break;
	}
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
