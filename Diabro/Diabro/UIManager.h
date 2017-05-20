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
	void update(const Ogre::FrameEvent&);

	void adjustHealthBar(Ogre::Real, Ogre::Real);
	void adjustStaminaBar(Ogre::Real, Ogre::Real);
	void showDialog(Ogre::String, Ogre::String);
	void destroyDialog();
	void appendDialogText(Ogre::String);
	static Ogre::Real calcBarSize(Ogre::Real, Ogre::Real, Ogre::Real);

	void showHUDText(Ogre::String);
	void showHUDText(Ogre::String,float);
	void hideHUDText();

private:
	DiabroUI::UIElementsManager*	_uiElementMgr;
	Ogre::SceneNode*			_uiNode;

	Ogre::RenderWindow*			_mWindow;

	DiabroUI::DialogTextBox*	_mDialogTextArea;
	
	DiabroUI::Bar*				_healthBarWidget;

	DiabroUI::HUDText*			_hudTextWidget;
	float						_hudTotalTimer;
	float						_hudTimer;
	bool						_hudTextWithTimeOn;

	Ogre::Real					_maxWidthBar;
	Ogre::Real					_heightBar;
};

#endif