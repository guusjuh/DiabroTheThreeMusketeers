#ifndef PLAYER_H_
#define PLAYER_H_

#pragma once
#include "Character.h"
#include "BaseEnemy.h"
#include "Npc.h"
#include "IPlayerEquipment.h"
#include "PlayerEquipment.h"

/// <summary>
/// The player character of the game. Only one exists. 
/// </summary>
/// <seealso cref="Character" />
class Player : public Character, public IQuestContent
{
	friend class QuestGenerator;

public:
	Player(Ogre::SceneNode*, Ogre::Entity*);
	~Player(){}

	void reset(Ogre::SceneNode*, Ogre::Entity*);

	Ogre::Real getSpeed() override { return _isRunning ? _runspeed : _movespeed; }
	float getMaxHealth() { return _maxHealth; }
	float getDamage() { return _damage; }
	void update(Ogre::Real) override;

	bool adjustHealth(float) override;
	bool lightAttack() override;
	void gainXP(int);
	
	bool isInDialog() { return _inDialog; }
	void dialogTriggered();

	void interactionTriggered();

	bool getInQuest() { return _inQuest; }
	void upgradeEquipment(PlayerUpgradeType type);

	void recieveItem() override;

	int healthUpgradeAmount() { return _healthUpgrades; }
	int damageUpgradeAmount() { return _damageUpgrades; }

	void setRunning(bool val) { _isRunning = val; }

private:
	Ogre::SceneNode* attackNode;

	static const float healPerFloorPercentage;

	void die() override;
	int curCityId;

	IPlayerEquipment* equipment;
	int _healthUpgrades;
	int _damageUpgrades;

	Npc* _nearbyNPC;
	bool _inDialog;
	void setNearbyNPC(Npc* newNPC);

	bool _sisNearby;
	void setSisterNearby(bool val);
	void findSister();

	bool _inBattle;
	float _inBattleTime;
	float _totalInBattleTime;
	void changeInBattle(bool val);
	//bool _canAttack;

	//for the animation
	int _rotAmount;

	float _runspeed;
	bool _isRunning;

	bool _inQuest;

	static const int LOW_HP;
	static const int HIGH_HP;
	static const int LOW_DMG;
	static const int HIGH_DMG;
};

#endif
