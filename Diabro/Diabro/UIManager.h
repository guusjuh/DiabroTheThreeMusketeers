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
	void adjustEnemyHealthBar(Ogre::Real, Ogre::Real);

	void showEnemyHealthBar();
	void hideEnemyHealthBar();
	static Ogre::Real calcBarSize(Ogre::Real, Ogre::Real, Ogre::Real);

	void showDialog(Ogre::String, Ogre::String);
	void hideDialog();
	void appendDialogText(Ogre::String);

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
	Ogre::Real					_maxWidthBar;

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

};

#endif