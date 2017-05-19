#ifndef QUEST_CONTENT_MANAGER_H
#define  QUEST_CONTENT_MANAGER_H

#pragma once
#include "QuestItemContainer.h"
#include "QuestItemGenerator.h"
#include "tinyxml2.h"
#include "City.h"

enum QuestContent {
	QuestItemQC = 0,
	ItemQC = 0,
	TownQC,
	HideOutQC,
	NPCQC,
	EnemyQC
};

class QuestContentManager {
public:
	QuestContentManager();
	~QuestContentManager();

	QuestItemContainer* getItemContainer() { return _itemContainer; };
	QuestItemGenerator* getItemGenerator() { return _itemGenerator; };

	//TODO: create locations and store/ref to them in this content manager
	std::vector<City*> cities;

private:
	QuestItemContainer* _itemContainer;
	QuestItemGenerator* _itemGenerator;

	void readFromXML();
	std::vector<BaseQuestItem*> readFromXMLQuestItemList(const char*, QuestItemType, tinyxml2::XMLElement*);
};

#endif