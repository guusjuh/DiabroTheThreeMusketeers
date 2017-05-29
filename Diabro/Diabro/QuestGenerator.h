#ifndef QUEST_GENERATOR
#define QUEST_GENERATOR
#include "Quest.h"
#include "Npc.h"
class BasicEnemy;

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

	std::pair<IQuestContent*, int> findTypeAndID(std::vector<std::pair<IQuestContent*, int>> searchSpace, QuestContent type, int id);
	Npc* getRandomNPC(Npc* pSourceNpc);
	BasicEnemy* getRandomEnemy();
	City* getRandomCity(RoomType type);

	PlayerUpgradeType generateRandomUpgrade();

	std::string getFilledTemplate(std::string templateString, 
		std::vector<std::pair<IQuestContent*, int>> concreteContent);

};

#endif