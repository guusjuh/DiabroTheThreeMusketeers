#ifndef NPC_H_
#define NPC_H_

#pragma once
#include "BaseNpc.h"

class Npc : public BaseNpc
{
public:
	Npc(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*);
	~Npc();

	static Ogre::String getMeshName() { return "penguin.mesh"; }

	void update(Ogre::Real) override;
	bool dialog(Ogre::Vector3);
	void toggleDialog();
	void continueDialog();
	void die() override;

private: 
	bool _inDialog;
	int _dialogCount;
	Ogre::String _startDialogText;
	Ogre::String _endDialogText;


};

#endif