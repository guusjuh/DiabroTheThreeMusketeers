#include "QuestItemGenerator.h"
#include "GameManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="QuestItemGenerator"/> class.
/// </summary>
QuestItemGenerator::QuestItemGenerator() { }

/// <summary>
/// Finalizes an instance of the <see cref="QuestItemGenerator"/> class.
/// </summary>
QuestItemGenerator::~QuestItemGenerator() { }

/// <summary>
/// Generates a random quest item of a specified type.
/// </summary>
/// <param name="pNode">The scene node to function as parent for the item.</param>
/// <returns>A pointer to the quest item.</returns>
QuestItem* QuestItemGenerator::generateItem(QuestItemType pType) {
	// the base quest item, contains information for the quest item to be generated. 
	QuestItem* baseItem;

	// possible items to return
	std::vector<QuestItem*> possibleItems = GameManager::getSingletonPtr()->getQuestManager()->getQuestContentManager()->getItemContainer()->getItemsOfType(pType);

	// random roll for the item of specified type.
	int randomRoll = (int)GameManager::getSingletonPtr()->getRandomInRange(0, possibleItems.size());

	// get the random item.
	baseItem = possibleItems[randomRoll];

	// create and return the item. 
	return new QuestItem((*baseItem));
}

/// <summary>
/// Generates a random quest item.
/// </summary>
/// <param name="pNode">The scene node to function as parent for the item.</param>
/// <returns>A pointer to the quest item.</returns>
QuestItem* QuestItemGenerator::generateItem() {
	// random roll the item type
	int randomRoll = (int)GameManager::getSingletonPtr()->getRandomInRange(0, AMOUNT_OF_QUEST_ITEM_TYPES);

	// generates random item of given type and returns
	return generateItem((QuestItemType)randomRoll);
}

/// <summary>
/// Generates a random quest item.
/// </summary>
/// <param name="pNode">The scene node to function as parent for the item.</param>
/// <param name="pRarity">The rarity.</param>
/// <returns>
/// A pointer to the quest item.
/// </returns>
QuestItem* QuestItemGenerator::generateItem(int pRarity) {
	// the base quest item, contains information for the quest item to be generated. 
	QuestItem* baseItem;

	// possible items to return
	std::vector<QuestItem*> possibleItems = GameManager::getSingletonPtr()->getQuestManager()->getQuestContentManager()->getItemContainer()->getItemsOfRarity(pRarity);

	// random roll for the item of specified type.
	int randomRoll = (int)GameManager::getSingletonPtr()->getRandomInRange(0, possibleItems.size());

	// get the random item.
	baseItem = possibleItems[randomRoll];

	// create and return the item. 
	return new QuestItem((*baseItem));
}

/// <summary>
/// Generates an amount of random quest items.
/// </summary>
/// <param name="pNode">The scene node to function as parent for the item.</param>
/// <param name="pAmount">The amount of items to generate.</param>
/// <returns>A vector with pointers to the quest items</returns>
std::vector<QuestItem*> QuestItemGenerator::generateItems(QuestItemType pType, int pAmount) {
	// the list to return
	std::vector<QuestItem*> returnList;

	// generate the x amount of quest items
	for (int i = 0; i < pAmount; ++i) {
		// generate 1 quest item and add it to the return list
		QuestItem* item = generateItem(pType);
		returnList.push_back(item);
	}

	// return the list
	return returnList;
}

/// <summary>
/// Generates an amount of random quest items of random types.
/// </summary>
/// <param name="pNode">The scene node to function as parent for the item.</param>
/// <param name="pAmount">The amount of items to generate.</param>
/// <returns>A vector with pointers to the quest items</returns>
std::vector<QuestItem*> QuestItemGenerator::generateItems(int pAmount) {
	// the list to return
	std::vector<QuestItem*> returnList;

	// generate the x amount of quest items
	for (int i = 0; i < pAmount; ++i) {
		// generate 1 quest item and add it to the return list
		QuestItem* item = generateItem();
		returnList.push_back(item);
	}

	// return the list
	return returnList;
}

/// <summary>
/// Generates an amount of random quest items of random types.
/// </summary>
/// <param name="pNode">The scene node to function as parent for the item.</param>
/// <param name="pRarity">The rarity.</param>
/// <param name="pAmount">The amount of items to generate.</param>
/// <returns>
/// A vector with pointers to the quest items
/// </returns>
std::vector<QuestItem*> QuestItemGenerator::generateItems(int pRarity, int pAmount) {
	// the list to return
	std::vector<QuestItem*> returnList;

	// generate the x amount of quest items
	for (int i = 0; i < pAmount; ++i) {
		// generate 1 quest item and add it to the return list
		QuestItem* item = generateItem(pRarity);
		returnList.push_back(item);
	}

	// return the list
	return returnList;
}

