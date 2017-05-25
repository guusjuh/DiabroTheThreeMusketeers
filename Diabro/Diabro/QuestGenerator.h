#ifndef QUEST_GENERATOR
#define QUEST_GENERATOR
#include "Quest.h"

/// <summary>
/// The generator for new quests. 
/// </summary>
class QuestGenerator {
public:
	QuestGenerator();
	~QuestGenerator();

	Quest generateQuest(BaseNpc* pSourceNpc, NeedType pMotivation);

private:
	Quest* generateAbstractQuest(Quest* pQuest, BaseNpc* pSourceNpc, NeedType pMotivation);
	Quest generateConcreteQuest(Quest pQuest, BaseNpc* pSourceNpc);
};

#endif