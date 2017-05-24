#include "ActionContainer.h"
#include "QuestManager.h"
#include "GameManager.h"
#include "tinyxml2.h"
#include <stdio.h>

ActionContainer::ActionContainer() {
	readFromXML();
}

ActionContainer::~ActionContainer() {
	for (int i = 0; i < _objects.size(); ++i) {
		if (_objects[i] != nullptr) {
			delete _objects[i];
		}
	}
}

void ActionContainer::readFromXML()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("../../XML/Actions.xml");
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement("ActionContainer");

	// if the rootnode is not empty
	if (rootNode) {
		tinyxml2::XMLElement* actionNode = rootNode->FirstChildElement("Action");

		for (actionNode; actionNode; actionNode = actionNode->NextSiblingElement()) {
			int id = actionNode->FirstChildElement("ID")->IntText();

			ActionType type = (ActionType)0;
			for (std::map<std::string, ActionType>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToActionType.begin(); 
				it != GameManager::getSingletonPtr()->getQuestManager()->stringToActionType.end(); ++it) {
				std::string typeString = actionNode->FirstChildElement("Type")->FirstChild()->ToText()->Value();
				if (typeString == it->first) {
					type = it->second;
				}
			}

			// pre conditions
			std::vector<PreconditionsType> preconditions;
			tinyxml2::XMLElement* listNode = actionNode->FirstChildElement("Preconditions");
			tinyxml2::XMLElement* preconNode = listNode->FirstChildElement("Precondition");
			for (preconNode; preconNode; preconNode = preconNode->NextSiblingElement())
			{
				std::string temp = preconNode->FirstChild()->ToText()->Value();

				for (std::map<std::string, PreconditionsType>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToPreconditionType.begin();
					it != GameManager::getSingletonPtr()->getQuestManager()->stringToPreconditionType.end(); ++it) {
					if (temp == it->first) {
						preconditions.push_back(it->second);
					}
				}
			}

			// post condition
			PostconditionType postcondition = (PostconditionType)0;
			for (std::map<std::string, PostconditionType>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToPostconditionType.begin(); it != GameManager::getSingletonPtr()->getQuestManager()->stringToPostconditionType.end(); ++it) {
				std::string typeString = actionNode->FirstChildElement("Postcondition")->FirstChild()->ToText()->Value();
				if (typeString == it->first) {
					postcondition = it->second;
				}
			}

			//required content
			std::map<QuestContent, int> questContent;
			listNode = actionNode->FirstChildElement("RequiredContent");
			tinyxml2::XMLElement* pairNode = listNode->FirstChildElement("Pair");
			for (pairNode; pairNode; pairNode = pairNode->NextSiblingElement())
			{
				std::string typeString = pairNode->FirstChildElement("Type")->FirstChild()->ToText()->Value();
				int nr = pairNode->FirstChildElement("Nr")->IntText();

				QuestContent content = (QuestContent)0;
				for (std::map<std::string, QuestContent>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.begin(); 
					it != GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.end(); ++it) {
					if (typeString == it->first) {
						content = it->second;
					}
				}

				questContent.insert(std::pair<QuestContent, int>(content, nr));
			}

			Action* action = new Action(id, type, preconditions, postcondition, questContent);
			_objects.push_back(action);
		}
	}
}
