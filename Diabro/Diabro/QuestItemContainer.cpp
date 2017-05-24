#include "QuestItemContainer.h"

/// <summary>
/// Initializes a new instance of the <see cref="QuestItemContainer"/> class.
/// </summary>
QuestItemContainer::QuestItemContainer() { }

/// <summary>
/// Finalizes an instance of the <see cref="QuestItemContainer"/> class.
/// </summary>
QuestItemContainer::~QuestItemContainer() {
	// delete each item
	for (int i = 0; i < _items.size(); ++i) {
		delete _items[i];
	}
}

std::vector<QuestItem*> QuestItemContainer::getItemsOfType(QuestItemType pType) {
	std::vector<QuestItem*> returnVector;
	
	for (int i = 0; i < _items.size(); ++i) {
		if(_items[i]->getItemType() == pType) {
			returnVector.push_back(_items[i]);
		}
	}

	return returnVector;
}