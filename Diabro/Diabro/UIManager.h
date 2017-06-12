#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#pragma once
#include "UIElements.h"
#include "EnemyUpgrade.h"

/// <summary>
/// The manager of the UI. 
/// </summary>
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
	static Ogre::Real calcLocatorPos(Ogre::Real pAngle, Ogre::Real pMaxSize);

	void showDialog(Ogre::String, Ogre::String);
	void hideDialog();
	void appendDialogText(Ogre::String);

	void showHUDText(Ogre::String);
	void showHUDText(Ogre::String, float);
	void hideHUDText();

	void updateMiniMapLocators();

	void setQuestOn(bool val);
	void setQuestTarget(Ogre::Vector3 position);

	void increaseFloorText();
	void resetFloorText();

	void setUpgradeText(UpgradeModifierType type);
	void resetUpgradeText();

	void showPauseScreen();
	void hidePauseScreen();
	
	void showMainMenu();
	void hideMainMenu();

private:
	DiabroUI::UIElementsManager*	_uiElementMgr;
	Ogre::SceneNode*			_uiNode;

	Ogre::RenderWindow*			_mWindow;

	bool						_dialogOn;
	DiabroUI::TextBox*	_mDialogTextArea;
	
	DiabroUI::MiniMap*			_miniMap;
	bool						_questOn;
	Ogre::Vector3				_questTargetPos;

	DiabroUI::Bar*				_playerHealthBarWidget;
	bool						_enemyBarOn;
	DiabroUI::Bar*				_enemyHealthBarWidget;
	Ogre::Real					_maxWidthBar;

	DiabroUI::Text*				_floorTextWidget;
	DiabroUI::Text*				_questGoalTextWidget;

	DiabroUI::UpgradeIcon*		_healthUpgradeIcon;
	DiabroUI::UpgradeIcon*		_dmgUpgradeIcon;

	bool						_hudTextOn;
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

	// PAUSE ELEMENTS
	DiabroUI::HUDText*			_pauseText;
	DiabroUI::TextBox*	_informationTextBox;

	// MAIN MENU ELEMENTS
	DiabroUI::HUDText*			_startGameTextElement;
	DiabroUI::ImageWidget*		_logoImg;

	bool showStoryText(std::vector<std::string>, float);

	void hideAllIngameElements();
	void showAllIngameElements();
};

#endif