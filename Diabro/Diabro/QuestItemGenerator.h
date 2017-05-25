#ifndef QUEST_ITEM_GEN_H
#define QUEST_ITEM_GEN_H

#pragma once
#include <vector>
#include "QuestItem.h"

class QuestItemGenerator {
public:
	QuestItemGenerator();
	~QuestItemGenerator();

	std::vector<QuestItem*> generateItems(QuestItemType, int);
	std::vector<QuestItem*> generateItems(int, int);
	std::vector<QuestItem*> generateItems(int);

	QuestItem* generateItem(QuestItemType);
	QuestItem* generateItem(int);
	QuestItem* generateItem();
};

#endif