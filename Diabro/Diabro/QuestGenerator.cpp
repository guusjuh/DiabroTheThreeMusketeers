#include "QuestGenerator.h"
#include "GameManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="QuestGenerator"/> class.
/// </summary>
QuestGenerator::QuestGenerator() {
	
}

/// <summary>
/// Finalizes an instance of the <see cref="QuestGenerator"/> class.
/// </summary>
QuestGenerator::~QuestGenerator() {
	
}

/// <summary>
/// Generates a new quest for a specified NPC and need.
/// </summary>
/// <param name="pSourceNpc">The source NPC for the quest.</param>
/// <param name="pMotivation">The motivation that the NPC has for the quest.</param>
/// <returns></returns>
Quest QuestGenerator::generateQuest(BaseNpc* pSourceNpc, NeedType pMotivation) {
	// the quest to generate and return
	Quest returnQuest;

	// returns an abstract quest
	returnQuest = (*generateAbstractQuest(&returnQuest, pSourceNpc, pMotivation));

	// returns a concrete quest
	returnQuest = (*generateConcreteQuest(&returnQuest, pSourceNpc));
	
	return returnQuest;
}

/// <summary>
/// Generates the abstract quest.
/// </summary>
/// <param name="pQuest">The quest to return.</param>
/// <param name="pSourceNpc">The source NPC for the quest.</param>
/// <param name="pMotivation">The motivation that the NPC has for the quest.</param>
/// <returns></returns>
Quest* QuestGenerator::generateAbstractQuest(Quest* pQuest, BaseNpc* pSourceNpc, NeedType pMotivation) {
	// find all possible strategies for this motivation
	std::vector<Strategy*> possibleStrategies;

	for (int i = 0; i < GameManager::getSingletonPtr()->getQuestManager()->getStrategyContainer()->GetObjects().size(); ++i) {
		if(GameManager::getSingletonPtr()->getQuestManager()->getStrategyContainer()->GetObjects()[i]->getMotivation() == pMotivation) {
			possibleStrategies.push_back(GameManager::getSingletonPtr()->getQuestManager()->getStrategyContainer()->GetObjects()[i]);
		}
	}

	// random roll and pick the strategy
	int randomRoll = GameManager::getSingletonPtr()->getRandomInRange(0, possibleStrategies.size());

	// copy actions
	std::vector<Action> tempActions = possibleStrategies[randomRoll]->getActionSequence();
	std::vector<Action> newActions;
	for (int i = 0; i < tempActions.size(); ++i) {
		newActions.push_back(Action(tempActions[i].getID(), tempActions[i].getType(), tempActions[i].getPreconditions(), 
			tempActions[i].getPostcondition(), tempActions[i].getRequiredContent()));
	}

	Strategy choosenStrat = Strategy(possibleStrategies[randomRoll]->getID(), possibleStrategies[randomRoll]->getName(), 
		possibleStrategies[randomRoll]->getNameQuest(), possibleStrategies[randomRoll]->getMotivation(),
		newActions, possibleStrategies[randomRoll]->getRarityPref());

	//TODO: improve selection of rewards
	float healthReward = (GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getMaxHealth() / 100) * 10;

	// assign values to the quest
	pQuest = new Quest(choosenStrat, pSourceNpc, healthReward);

	return pQuest;
}

/// <summary>
/// Generates the concrete quest.
/// </summary>
/// <param name="pQuest">The quest to return.</param>
/// <param name="pSourceNpc">The source NPC for the quest.</param>
/// <returns></returns>
Quest* QuestGenerator::generateConcreteQuest(Quest* pQuest, BaseNpc* pSourceNpc) {




	return pQuest;
}



