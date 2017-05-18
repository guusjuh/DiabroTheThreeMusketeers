#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include <map>
#include "QuestContentManager.h"
#include "Action.h"
#include "ActionContainer.h"

class QuestManager {
public:
	QuestManager() {
		stringToActionType.insert(std::pair<std::string, ActionType>("Export", Export));
		stringToActionType.insert(std::pair<std::string, ActionType>("Exchange", Exchange));
		stringToActionType.insert(std::pair<std::string, ActionType>("Explore", Explore));
		stringToActionType.insert(std::pair<std::string, ActionType>("Get", Get));
		stringToActionType.insert(std::pair<std::string, ActionType>("Give", Give));
		stringToActionType.insert(std::pair<std::string, ActionType>("Goto", Goto));
		stringToActionType.insert(std::pair<std::string, ActionType>("Kill", Kill));
		stringToActionType.insert(std::pair<std::string, ActionType>("Listen", Listen));
		stringToActionType.insert(std::pair<std::string, ActionType>("Report", Report));

		stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("SomebodyThere", SomebodyThere));
		stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("SomethingThere", SomethingThere));
		stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("KnowWhereToGo", KnowWhereToGo));
		stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("YouItemOfInterest", YouItemOfInterest));
		stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("TheyItemOfInterest", TheyItemOfInterest));
		stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("BothItemOfInterest", BothItemOfInterest));

		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("Accompanied", Accompanied));
		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("YouHaveTheItem", YouHaveTheItem));
		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("TheyHaveTheItem", TheyHaveTheItem));
		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("BothHaveTheItem", BothHaveTheItem));
		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("YouInfo", YouInfo));
		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("TheyInfo", TheyInfo));
		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("YouThere", YouThere));
		stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("TheyDead", TheyDead));
		
		actionContainer = new ActionContainer();

	}

	QuestContentManager* getQuestContentManager(void) { return _questContentManager; }
	ActionContainer* actionContainer;
	static std::map<std::string, ActionType> stringToActionType;
	static std::map<std::string, PreconditionsType> stringToPreconditionType;
	static std::map<std::string, PostconditionType> stringToPostconditionType;

private:
	QuestContentManager* _questContentManager;

};

#endif