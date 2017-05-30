#ifndef QUEST_MANAGER_H
#define QUEST_MANAGER_H

#include <map>
#include "QuestContentManager.h"
#include "Action.h"
#include "ActionContainer.h"
#include "Needs.h"
#include "StrategyContainer.h"
#include "Quest.h"
#include "QuestGenerator.h"

/// <summary>
/// The manager for the quests. 
/// </summary>
class QuestManager {
public:
	QuestManager();
	~QuestManager(){}

	void Initialize()
	{
		_actionContainer = new ActionContainer();
		_strategyContainer = new StrategyContainer();
		_questContentManager = new QuestContentManager();
		_questGenerator = new QuestGenerator();
	}  

	QuestContentManager* getQuestContentManager(void) { return _questContentManager; }
	ActionContainer* getActionContainer(void) { return _actionContainer; }
	StrategyContainer* getStrategyContainer(void) { return _strategyContainer; }
	Quest* generateQuest(Npc* pSourceNpc, NeedType pMotivation);

	bool questCanStart() { return _currentQuest == nullptr ? true : false; }
	std::string startQuest(Npc* sourceNpc);

	std::string obtainDialog(IQuestContent* client);

	// maps a string to an enum to enable parsing of xml files. 
	std::map<std::string, ActionType> stringToActionType;
	std::map<std::string, PreconditionsType> stringToPreconditionType;
	std::map<std::string, PostconditionType> stringToPostconditionType;
	std::map<std::string, QuestItemType> stringToQuestItemType;   
	std::map<std::string, QuestContent> stringToQuestContentType;
	std::map<std::string, NeedType> stringToNeedType;

private:
	QuestContentManager* _questContentManager;
	ActionContainer* _actionContainer;
	StrategyContainer* _strategyContainer;
	QuestGenerator* _questGenerator;

	Quest* _currentQuest;
	std::vector<Quest> _quests;
};

#endif