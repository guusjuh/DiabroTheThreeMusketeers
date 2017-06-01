#ifndef PLAYER_H_
#define PLAYER_H_

#pragma once
#include "Character.h"
#include "BasicEnemy.h"
#include "Npc.h"
#include "IPlayerEquipment.h"
#include "PlayerEquipment.h"

/// <summary>
/// The player character of the game. Only one exists. 
/// </summary>
/// <seealso cref="Character" />
class Player : public Character
{
	friend class QuestGenerator;

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
	float angleBetween(Ogre::Vector3);

	bool getInQuest() { return _inQuest; }
	void toggleInQuest();
	void upgradeEquipment(PlayerUpgradeType type);

	void recieveItem() override;

private:
	void die() override;

	int _currentXP;
	int _xpTillNextLevel;

	IPlayerEquipment* equipment;

	Npc* _nearbyNPC;
	bool _inDialog;
	void setNearbyNPC(Npc* newNPC);

	bool _inBattle;
	float _inBattleTime;
	float _totalInBattleTime;
	void changeInBattle(bool val);

	int calcXpTillLevel(int);
	void levelUp();
	bool _canAttack;

	bool _inQuest;

	static const int LOW_HP;
	static const int HIGH_HP;
	static const int LOW_DMG;
	static const int HIGH_DMG;
};

#endif
