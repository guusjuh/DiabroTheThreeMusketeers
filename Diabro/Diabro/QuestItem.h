#ifndef BASE_QUEST_ITEM_H
#define BASE_QUEST_ITEM_H

#pragma once
#include <OgrePrerequisites.h>
#include "IQuestContent.h"

enum QuestItemType {
	Book,
	RareBook,
	Food,
	RawMaterials,
	Gem,
	Mysterious,

	AMOUNT_OF_QUEST_ITEM_TYPES
};

class QuestItem : public IQuestContent
{
public:
	QuestItem() 
		: _name(""), _sprName(""), _itemType((QuestItemType)0) {
		switch(_itemType) {
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
			_rarity = 1;
			break;
		default:
			_rarity = 0;
			break;
		}
	}
	
	QuestItem(std::string pName, std::string pSprName, QuestItemType pType)
		: _name(pName), _sprName(pSprName), _itemType(pType) {
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
			_rarity = 1;
			break;
		default:
			_rarity = 0;
			break;
		}
	}
	
	~QuestItem() {}

	std::string getName() { return _name; }
	std::string getSpriteName() { return _sprName; }
	QuestItemType getItemType() { return _itemType; }
	int getRarity() { return _rarity; }

private:
	std::string _name;
	std::string _sprName;
	QuestItemType _itemType;
	int _rarity;
};

#endif