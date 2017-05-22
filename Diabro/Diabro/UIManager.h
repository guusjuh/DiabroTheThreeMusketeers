#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#pragma once
#include <OgreBillboardSet.h>
#include "BaseApplication.h"
#include "SdkTrays.h"
#include "UIElements.h"

class UIManager
{
public:
	UIManager();

	void init();
	void setupUI();

	void startUpdate(const Ogre::FrameEvent&);
	void inGameUpdate(const Ogre::FrameEvent&);
	void endUpdate(const Ogre::FrameEvent&);
	void diedUpdate(const Ogre::FrameEvent&);

	void adjustHealthBar(Ogre::Real, Ogre::Real);
	void adjustStaminaBar(Ogre::Real, Ogre::Real);
	void showDialog(Ogre::String, Ogre::String);
	void destroyDialog();
	void appendDialogText(Ogre::String);
	static Ogre::Real calcBarSize(Ogre::Real, Ogre::Real, Ogre::Real);

	void showHUDText(Ogre::String);
	void showHUDText(Ogre::String, float);
	void hideHUDText();

private:
	DiabroUI::UIElementsManager*	_uiElementMgr;
	Ogre::SceneNode*			_uiNode;

	Ogre::RenderWindow*			_mWindow;

	DiabroUI::DialogTextBox*	_mDialogTextArea;

	DiabroUI::Bar*				_playerHealthBarWidget;
	DiabroUI::Bar*				_enemyHealthBarWidget;

	DiabroUI::HUDText*			_hudTextWidget;
	float						_hudTotalTimer;
	float						_hudTimer;
	bool						_hudTextWithTimeOn;

	std::vector<std::string>	_startGameText;
	std::vector<std::string>	_startLevelText;
	std::vector<std::string>	_endLevelText;
	std::vector<std::string>	_diedText;
	bool						_storyTextOn;
	bool						_first;

	bool showStoryText(std::vector<std::string>, float);

	Ogre::Real					_maxWidthBar;
	Ogre::Real					_heightBar;

/*public:
	void startState();
	void update(const Ogre::FrameEvent&);

	bool checkforHidingHudText(float);

	void adjustHealthBar(Ogre::Real, Ogre::Real);
	void adjustEnemyHealthBar(Ogre::Real, Ogre::Real);

	void showEnemyHealthBar();
	void hideEnemyHealthBar();


	void showDialog(Ogre::String, Ogre::String);
	void destroyDialog();
	void appendDialogText(Ogre::String);

	void showHUDText(Ogre::String);
	void showHUDText(Ogre::String,float);
	void hideHUDText();

private:
	DiabroUI::HUDText*			_hudTextWidget;
	float						_hudTotalTimer;
	float						_hudTimer;
	bool						_hudTextWithTimeOn;

	std::vector<std::string>	_startGameText;
	std::vector<std::string>	_startLevelText;
	std::vector<std::string>	_endLevelText;
	std::vector<std::string>	_diedText;
	bool						_storyTextOn;
	bool						_first;
	bool showStoryText(std::vector<std::string>, float);
*/
};

#endif