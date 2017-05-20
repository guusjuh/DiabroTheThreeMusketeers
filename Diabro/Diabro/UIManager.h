#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#pragma once
#include <OgreBillboardSet.h>
#include "BaseApplication.h"
#include "SdkTrays.h"

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

private:
	OgreBites::SdkTrayManager*	_mSdkTrayMgr;
	OgreBites::SdkTrayListener* _mSdkTrayListener;

	Ogre::RenderWindow*			_mWindow;
	OgreBites::InputContext     _mInputContext;

	OgreBites::DialogTextBox*	_mDialogTextArea;
	
	OgreBites::DecorWidget*		_healthBarWidget;
	OgreBites::DecorWidget*		_staminaBarWidget;

	OgreBites::HUDText*			_hudTextWidget;
	float						_hudTotalTimer;
	float						_hudTimer;
	bool						_hudTextOn;
	Ogre::SceneNode*			_uiNode;

	Ogre::Real					_maxWidthBar;
	Ogre::Real					_heightBar;

	void hideHUDText();

};

#endif