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

	void reset(Ogre::SceneNode*, Ogre::Entity*);

	float getMaxHealth() { return _maxHealth; }
	void update(Ogre::Real) override;

	bool adjustHealth(float) override;
	bool lightAttack() override;
	void gainXP(int);
	
	void dialogTriggered();

private:
	void die() override;

	int _currentXP;
	int _xpTillNextLevel;

	Npc* _nearbyNPC;
	bool _inDialog;

	bool _inBattle;
	float _inBattleTime;
	float _totalInBattleTime;

	int calcXpTillLevel(int);
	void levelUp();
	bool _canAttack;
};

#endif
