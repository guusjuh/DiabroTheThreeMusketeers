#pragma once
#include "IEnemyEquipment.h"

class EnemyUpgrade : public IEnemyEquipment
{
public:
	EnemyUpgrade(IEnemyEquipment* base, float healthMod = 0, float damageMod = 0, float noticeDistMod = 0, UpgradeModifierType type = (UpgradeModifierType)0);
	~EnemyUpgrade();

	float healthMod;
	float damageMod;
	float noticeDistMod;

	float getHealth() override;
	float getDamage() override;
	float getNoticeDist() override;

	IEnemyEquipment* getBase() override { return equipment; }
	UpgradeModifierType getType() override { return type; }

	bool isBase() override {return false;}

protected:
	IEnemyEquipment* equipment;
	UpgradeModifierType type;

	IEnemyEquipment* removeUpgrades() override;
};

class EnemyNoticeDistUpgrade : public EnemyUpgrade
{
public:
	EnemyNoticeDistUpgrade(IEnemyEquipment* equipment, float noticeDistMod, UpgradeModifierType type);
	~EnemyNoticeDistUpgrade();

	float getNoticeDist() override;
};

class EnemyHealthUpgrade : public EnemyUpgrade
{
public:
	EnemyHealthUpgrade(IEnemyEquipment* equipment, float healthMod, UpgradeModifierType type);
	~EnemyHealthUpgrade();

	float getHealth() override;
};

class EnemyDamageUpgrade : public EnemyUpgrade
{
public:
	EnemyDamageUpgrade(IEnemyEquipment* equipment, float damageMod, UpgradeModifierType type);
	~EnemyDamageUpgrade();

	float getDamage() override;
};

