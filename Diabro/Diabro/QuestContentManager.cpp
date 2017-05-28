#include "QuestContentManager.h"
#include "tinyxml2.h"
#include <stdio.h>
#include "GameManager.h"
#include "Debug.h"

/// <summary>
/// Initializes a new instance of the <see cref="QuestContentManager"/> class.
/// </summary>
QuestContentManager::QuestContentManager() {
	_itemContainer = new QuestItemContainer();
	_itemGenerator = new QuestItemGenerator();

	readFromXML();

	//find all cities and add them to correct list
	for(int i = 0; i < GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities.size(); ++i) {
		if(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i].typeFlag == CityRT) {
			_NPCCities.push_back(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i]);
		} else if (GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i].typeFlag == HideoutRT) {
			_enemyCities.push_back(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i]);
		}
	}
}

/// <summary>
/// Finalizes an instance of the <see cref="QuestContentManager"/> class.
/// </summary>
QuestContentManager::~QuestContentManager() {
	delete _itemContainer;
	delete _itemGenerator;
}

/// <summary>
/// Reads from QuestItems.XML to fill the quest item container.
/// </summary>
void QuestContentManager::readFromXML() {
	// load the document and find the rootnode
	tinyxml2::XMLDocument doc;
	doc.LoadFile("../../XML/QuestItems.xml");
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement("QuestItemContainer");

	// load questitems from xml
	std::vector<QuestItem*> listItems = readFromXMLQuestItemList(rootNode);
	_itemContainer->setItems(listItems);
	listItems.clear();
}

/// <summary>
/// Reads from QuestItems.XML the list of items.
/// </summary>
/// <param name="listNode">The root node of the XML.</param>
/// <returns></returns>
std::vector<QuestItem*> QuestContentManager::readFromXMLQuestItemList(tinyxml2::XMLElement* listNode) {
	// the vector to return
	std::vector<QuestItem*> listItems;

	// if the rootnode is not empty
	if (listNode) {

		// get the first child node, e.g. the first item
		tinyxml2::XMLElement* itemNode = listNode->FirstChildElement("BaseQuestItem");

		// declare two empty strings for the name and sprite name of each item to reuse
		std::string name = "";
		std::string sprName = "";

		// foreach item node
		for (itemNode; itemNode; itemNode = itemNode->NextSiblingElement()) {

			// get the name and spritename attributes
			name = itemNode->FirstChildElement("Name")->FirstChild()->ToText()->Value();
			sprName = itemNode->FirstChildElement("SpriteName")->FirstChild()->ToText()->Value();

			QuestItemType type = (QuestItemType)0;
			for (std::map<std::string, QuestItemType>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToQuestItemType.begin();
				it != GameManager::getSingletonPtr()->getQuestManager()->stringToQuestItemType.end(); ++it) {
				std::string typeString = itemNode->FirstChildElement("Type")->FirstChild()->ToText()->Value();
				if (typeString == it->first) {
					type = it->second;
				}
			}

			// create the item
			QuestItem* tempItem = new QuestItem(name, sprName, type);

			// add the item to the return list
			listItems.push_back(tempItem);
		}
	}
	// the rootnode was empty
	else {
		Debug("Failed loading.");
	}

	// return the list of items 
	return listItems;
}