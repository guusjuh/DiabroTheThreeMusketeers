#include "StrategyContainer.h"
#include "QuestManager.h" 
#include "GameManager.h"
#include "tinyxml2.h"
#include <stdio.h>

/// <summary>
/// Initializes a new instance of the <see cref="StrategyContainer"/> class.
/// </summary>
StrategyContainer::StrategyContainer() {
	readFromXML();
}

/// <summary>
/// Finalizes an instance of the <see cref="StrategyContainer"/> class.
/// </summary>
StrategyContainer::~StrategyContainer() {
	for (int i = 0; i < _objects.size(); ++i) {
		if (_objects[i] != nullptr) {
			delete _objects[i];
		}
	}
}

/// <summary>
/// Reads all strategies from XML.
/// </summary>
void StrategyContainer::readFromXML()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("../../XML/Strategies.xml");
	tinyxml2::XMLElement* rootNode = doc.FirstChildElement("StrategyContainer");

	if (rootNode)
	{
		tinyxml2::XMLElement* stratNode = rootNode->FirstChildElement("Strategy");

		for (stratNode; stratNode; stratNode = stratNode->NextSiblingElement())
		{
			// id
			int id = stratNode->FirstChildElement("ID")->IntText();

			// name strat
			std::string nameStrat = stratNode->FirstChildElement("NameStrat")->FirstChild()->ToText()->Value();

			// quest name
			QuestName questName;
			tinyxml2::XMLElement* questNameNode = stratNode->FirstChildElement("QuestName");

			// since the pre or post string can be null, first check for that 
			tinyxml2::XMLNode* preNode = questNameNode->FirstChildElement("pre")->FirstChild();
			questName.preString = preNode == nullptr ? "" : preNode->ToText()->Value();

			tinyxml2::XMLNode* postNode = questNameNode->FirstChildElement("post")->FirstChild();
			questName.postString = postNode == nullptr ? "" : postNode->ToText()->Value();
			for (std::map<std::string, QuestContent>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.begin();
				it != GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.end(); ++it) {
				std::string tempTemplate = questNameNode->FirstChildElement("template")->FirstChild()->ToText()->Value();
				if (tempTemplate == it->first) {
					questName.templateContent = it->second;
				}
			}			

			// need type
			NeedType needType = (NeedType)0;
			for (std::map<std::string, NeedType>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToNeedType.begin(); 
				it != GameManager::getSingletonPtr()->getQuestManager()->stringToNeedType.end(); ++it) {
				std::string typeString = stratNode->FirstChildElement("NeedType")->FirstChild()->ToText()->Value();
				if (typeString == it->first) {
					needType = it->second;
				}
			}

			// action list
			std::vector<Action> actions;
			tinyxml2::XMLElement* actionsNode = stratNode->FirstChildElement("Actions");
			tinyxml2::XMLElement* actionNode = actionsNode->FirstChildElement("Action");
			for (actionNode; actionNode; actionNode = actionNode->NextSiblingElement())
			{
				Action tempAction;
				int nrAction = actionNode->FirstChildElement("ID")->IntText();
				
				std::vector<Action*> copy = GameManager::getSingletonPtr()->getQuestManager()->getActionContainer()->GetObjects();
				for (int i = 0; i < copy.size(); ++i)
				{
					if (copy[i]->getID() == nrAction)
					{
						tempAction = Action((*copy[i]));
					}
				}

				// get content node
				tinyxml2::XMLElement* contentNode = actionNode->FirstChildElement("Content");
				int counter = 0;

				// for loop till no next element
				for (contentNode; contentNode; contentNode = contentNode->NextSiblingElement()) 
				{
					// get content from node
					int id = contentNode->IntText();

					// store content in required content map
					tempAction._requiredContent[counter].second = id;

					counter++;
				}
				
				actions.push_back(tempAction);
			}

			// rarity pref
			int rarityPref = stratNode->FirstChildElement("RarityPref")->IntText();

			Strategy* strategy = new Strategy(id, nameStrat, questName, needType, actions, rarityPref);
			_objects.push_back(strategy);
		}

	}
}