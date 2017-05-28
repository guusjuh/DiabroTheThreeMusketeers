#ifndef QUEST_GENERATOR
#define QUEST_GENERATOR
#include "Quest.h"
#include "Npc.h"
/// <summary>
/// The generator for new quests. 
/// </summary>
class QuestGenerator {
public:
	QuestGenerator();
	~QuestGenerator();

	Quest generateQuest(Npc* pSourceNpc, NeedType pMotivation);

private:
	Quest* generateAbstractQuest(Quest* pQuest, Npc* pSourceNpc, NeedType pMotivation);
	Quest* generateConcreteQuest(Quest* pQuest, Npc* pSourceNpc);

	Npc* getRandomNPC();
};

#endif