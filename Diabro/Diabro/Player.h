#ifndef PLAYER_H_
#define PLAYER_H_

#pragma once
#include "Character.h"
#include "BasicEnemy.h"
#include "Npc.h"

class Player : public Character
{
public:
	Player(Ogre::SceneNode*, Ogre::Entity*);
	~Player(){}

	float getMaxHealth() { return _maxHealth; }
	void update(Ogre::Real) override;


	bool adjustHealth(float) override;
	bool lightAttack() override;
	void gainXP(int);
	
	void dialogTriggered();

private:
	int _currentXP;
	int _xpTillNextLevel;

	Npc* _nearbyNPC;
	bool _inDialog;

	int calcXpTillLevel(int);
	void levelUp();
	bool _canAttack;
};

#endif
