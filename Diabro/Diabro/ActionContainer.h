#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include "QuestManager.h"
#include "GenericContainer.h"
#include "Action.h"
#include "tinyxml2.h"

class ActionContainer : GenericContainer<Action> {
public:
	ActionContainer() {
		readFromXML();
	}

	~ActionContainer() {
		GenericContainer<Action>::~GenericContainer();
	}

	void readFromXML() {
		tinyxml2::XMLDocument doc;
		doc.LoadFile("../../Actions.xml");

		tinyxml2::XMLElement* rootNode = doc.FirstChildElement("ActionContainer");

		// if the rootnode is not empty
		if (rootNode) {
			tinyxml2::XMLElement* actionNode = rootNode->FirstChildElement("Action");

			int id = 0;
			ActionType type = (ActionType)0;
			std::vector<PreconditionsType> preconditions;
			PostconditionType postcondition = (PostconditionType)0;

			for (actionNode; actionNode; actionNode = actionNode->NextSiblingElement()) {
				id = actionNode->FirstChildElement("ID")->FirstChild()->ToElement()->IntText();
				
				for (std::map<std::string, ActionType>::iterator it = QuestManager::stringToActionType.begin(); it != QuestManager::stringToActionType.end(); ++it) {
					std::string typeString = actionNode->FirstChildElement("Type")->FirstChild()->ToText()->Value();
					if (typeString == it->first) {
						type = it->second;
					}
				}

				// preconditions

				for (std::map<std::string, PostconditionType>::iterator it = QuestManager::stringToPostconditionType.begin(); it != QuestManager::stringToPostconditionType.end(); ++it) {
					std::string typeString = actionNode->FirstChildElement("Postcondition")->FirstChild()->ToText()->Value();
					if (typeString == it->first) {
						postcondition = it->second;
					}
				}
				
				Action* action = new Action(id, type, preconditions, postcondition);
				_objects.push_back(action);
			}
		}
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		std::cout << "Action 1:" << _objects[0]->GetType() << std::endl;
		fclose(fp);
#endif
	}
};

#endif