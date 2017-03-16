#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#pragma once
#include <OgreBillboardSet.h>
#include "BaseApplication.h"

class UIManager
{
public:
	UIManager();
	
	void init();
	void setupUI();

	void adjustHealthBar(Ogre::Real value, Ogre::Real maxValue);
	void adjustStaminaBar(Ogre::Real value, Ogre::Real maxValue);
	Ogre::Real calcBarSize(Ogre::Real value, Ogre::Real maxValue, Ogre::Real maxSize);

private:
	Ogre::Billboard* setupUIBar(Ogre::String, Ogre::SceneNode*, Ogre::BillboardOrigin, Ogre::String, Ogre::Vector3, Ogre::Vector3);
	
	Ogre::SceneNode* _uiNode;

	Ogre::Billboard* _healthBar;
	Ogre::Billboard* _staminaBar;

	Ogre::Real _maxWidthBar;
	Ogre::Real _heightBar;
};

#endif