#include "EnemyUpgrade.h"

EnemyUpgrade::EnemyUpgrade(IEnemyEquipment* base, float healthMod, float damageMod, float noticeDistMod, UpgradeModifierType type)
	: equipment(base), healthMod(healthMod), damageMod(damageMod), noticeDistMod(noticeDistMod), type(type) { }

EnemyUpgrade::~EnemyUpgrade() { }

float EnemyUpgrade::getHealth() { return equipment->getHealth() + healthMod; }
float EnemyUpgrade::getDamage() { return equipment->getDamage() + damageMod; }
float EnemyUpgrade::getNoticeDist() { return equipment->getNoticeDist() + noticeDistMod; }

IEnemyEquipment* EnemyUpgrade::removeUpgrades() {
	if (equipment->isBase()) {
		return equipment;
	}
	else {
		return equipment->removeUpgrades();
	}
}

// health
EnemyHealthUpgrade::EnemyHealthUpgrade(IEnemyEquipment* equipment, float healthMod, UpgradeModifierType type)
	: EnemyUpgrade(equipment, healthMod, 0, 0, type) { }

EnemyHealthUpgrade::~EnemyHealthUpgrade() { }

float EnemyHealthUpgrade::getHealth() { return equipment->getHealth() + healthMod; }

// dmg
EnemyDamageUpgrade::EnemyDamageUpgrade(IEnemyEquipment* equipment, float damageMod, UpgradeModifierType type)
	: EnemyUpgrade(equipment, 0, damageMod, 0, type) { }

EnemyDamageUpgrade::~EnemyDamageUpgrade() { }

float EnemyDamageUpgrade::getDamage() { return equipment->getDamage() + damageMod; }

// noticeDist
EnemyNoticeDistUpgrade::EnemyNoticeDistUpgrade(IEnemyEquipment* equipment, float noticeDistMod, UpgradeModifierType type)
	: EnemyUpgrade(equipment, 0, 0, noticeDistMod, type) { }

EnemyNoticeDistUpgrade::~EnemyNoticeDistUpgrade() { }

float EnemyNoticeDistUpgrade::getNoticeDist() { return equipment->getNoticeDist() + noticeDistMod; }

