#ifndef PLAYER_H_
#define PLAYER_H_

#pragma once
#include "Character.h"
#include "BasicEnemy.h"

class Player : public Character
{
public:
	Player(Ogre::SceneNode*, Ogre::Entity*);
	~Player(){}

	bool initialize() override;
	bool adjustHealth(float) override;
	bool lightAttack() override;
	void gainXP(int);

private:
	int _currentXP;
	int _xpTillNextLevel;

	int calcXpTillLevel(int);
	void levelUp();
	bool _canAttack;
};

#endif
