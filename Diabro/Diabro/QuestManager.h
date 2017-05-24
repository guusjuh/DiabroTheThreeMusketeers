#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include <map>
#include "QuestContentManager.h"
#include "Action.h"
#include "ActionContainer.h"
#include "Needs.h"
#include "StrategyContainer.h"

class QuestManager {
public:
	QuestManager();
	~QuestManager(){}

	void Initialize()
	{
		actionContainer = new ActionContainer();
		strategyContainer = new StrategyContainer();
		_questContentManager = new QuestContentManager();
	}

	QuestContentManager* getQuestContentManager(void) { return _questContentManager; }
	ActionContainer* actionContainer;
	StrategyContainer* strategyContainer;

	std::map<std::string, ActionType> stringToActionType;
	std::map<std::string, PreconditionsType> stringToPreconditionType;
	std::map<std::string, PostconditionType> stringToPostconditionType;
	std::map<std::string, QuestItemType> stringToQuestItemType;   
	std::map<std::string, QuestContent> stringToQuestContentType;
	std::map<std::string, NeedType> stringToNeedType;

private:
	QuestContentManager* _questContentManager;

};

#endif