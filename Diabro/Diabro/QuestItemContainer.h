#ifndef QUEST_ITEM_CONTAINER_H
#define QUEST_ITEM_CONTAINER_H

#pragma once
#include <vector>
#include "QuestItem.h"

class QuestItemContainer {
public:
	QuestItemContainer();
	~QuestItemContainer();

	int itemAmount() { return _items.size(); }
	std::vector<QuestItem*> getItems() { return  _items; }
	void setItems(std::vector<QuestItem*> pItems) { _items = pItems; }

	std::vector<QuestItem*> getItemsOfType(QuestItemType pType);

private:
	std::vector<QuestItem*> _items;
};

#endif 