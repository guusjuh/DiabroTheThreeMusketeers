#ifndef QUEST_CONTENT_MANAGER_H
#define  QUEST_CONTENT_MANAGER_H

#pragma once
#include "QuestItemContainer.h"
#include "QuestItemGenerator.h"
#include "tinyxml2.h"
#include "City.h"
#include "QuestItem.h"

enum QuestContent {
	QuestItemQC = 0,
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
	std::vector<City*> getNPCCities() { return _NPCCities; }
	std::vector<City*> getEnemyCities() { return _enemyCities; }

private:
	QuestItemContainer* _itemContainer;
	QuestItemGenerator* _itemGenerator;
	std::vector<City*> _NPCCities;
	std::vector<City*> _enemyCities;

	void readFromXML();
	std::vector<QuestItem*> readFromXMLQuestItemList(tinyxml2::XMLElement*);
};

#endif