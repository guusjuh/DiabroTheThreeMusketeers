#ifndef BASE_QUEST_ITEM_H
#define BASE_QUEST_ITEM_H

#pragma once
#include <OgrePrerequisites.h>
#include "IQuestContent.h"

/// <summary>
/// Enum for the types of quest items. 
/// </summary>
enum QuestItemType {
	Book,
	RareBook,
	Food,
	RawMaterials,
	Gem,
	Mysterious,

	AMOUNT_OF_QUEST_ITEM_TYPES
};

/// <summary>
/// A item to be used in a quest. 
/// </summary>
/// <seealso cref="IQuestContent" />
class QuestItem : public IQuestContent
{
public:
	QuestItem() 
		: _name(""), _sprName(""), _itemType((QuestItemType)0), _rarity(0) { }
	
	QuestItem(std::string pName, std::string pSprName, QuestItemType pType)
		: _name(pName), _sprName(pSprName), _itemType(pType) {
		_relevantForAction = false;

		determineRarity();
	}
	
	~QuestItem() {}

	std::string getName() { return _name; }
	std::string getSpriteName() { return _sprName; }
	QuestItemType getItemType() { return _itemType; }
	int getRarity() { return _rarity; }

	QuestContent getType() override { return QuestItemQC; }
	Ogre::Vector3 getQuestPosition() override { return Ogre::Vector3(0, 0, 0); }

private:
	std::string _name;
	std::string _sprName;
	QuestItemType _itemType;
	int _rarity;

	void determineRarity() {
		switch (_itemType) {
		case Book:
			_rarity = 0;
			break;
		case Food:
			_rarity = 0;
			break;
		case RareBook:
			_rarity = 1;
			break;
		case RawMaterials:
			_rarity = 0;
			break;
		case Gem:
			_rarity = 1;
			break;
		case Mysterious:
			_rarity = 2;
			break;
		default:
			_rarity = 0;
			break;
		}
	}
};

#endif