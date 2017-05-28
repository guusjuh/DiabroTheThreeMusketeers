#include "QuestGenerator.h"
#include "GameManager.h"
#include "Debug.h"

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
Quest QuestGenerator::generateQuest(Npc* pSourceNpc, NeedType pMotivation) {
	// the quest to generate and return
	Quest returnQuest;

	// returns an abstract quest
	returnQuest = (*generateAbstractQuest(&returnQuest, pSourceNpc, pMotivation));

	// returns a concrete quest
	returnQuest = (*generateConcreteQuest(&returnQuest, pSourceNpc));
	
	// update all entities

	return returnQuest;
}

/// <summary>
/// Generates the abstract quest.
/// </summary>
/// <param name="pQuest">The quest to return.</param>
/// <param name="pSourceNpc">The source NPC for the quest.</param>
/// <param name="pMotivation">The motivation that the NPC has for the quest.</param>
/// <returns></returns>
Quest* QuestGenerator::generateAbstractQuest(Quest* pQuest, Npc* pSourceNpc, NeedType pMotivation) {
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
		newActions.push_back(Action(tempActions[i].getID(), tempActions[i].getType(), 
			tempActions[i].getPreconditions(), 
			tempActions[i].getPostcondition(), tempActions[i].getRequiredContentTypes()));
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
Quest* QuestGenerator::generateConcreteQuest(Quest* pQuest, Npc* pSourceNpc) {
	// define lists to keep track of id's
	std::vector<std::pair<IQuestContent*, int>> contentIDs;
	int friendlies, enemies, cities, hideouts, items = 1;

	contentIDs.push_back(std::pair<IQuestContent*, int>(pSourceNpc, friendlies));
	friendlies++;

	contentIDs.push_back(std::pair<IQuestContent*, int>(pSourceNpc->getHomeTown(), cities));
	cities++;

	// for each action
	std::vector<Action> actions = pQuest->_strategy.getActionSequence();
	for (int i = 0; i < actions.size(); ++i) {
		std::vector<std::pair<IQuestContent*, int>> concreteActionContent;
		Action* thisAction = &(actions[i]);

		// for each content required by the action
		for (int j = 0; j < thisAction->_requiredContent.size(); ++j) {
			QuestContent type = thisAction->_requiredContent[j].first;
			int id = thisAction->_requiredContent[j].second;

			// switch on the given item type
			switch (type) {
			case NPCQC:
				// if this id is already used in this quest
				if(id < friendlies) {
					for (int k = 0; k < contentIDs.size(); ++k) {
						if(contentIDs[k].first->getType() == NPCQC && contentIDs[k].second == id) {
							concreteActionContent.push_back(contentIDs[k]);
						}
					}
				}
				// else, a new npc has to be found is a different city
				else {
					Npc* randomNpc;

					std::vector<Npc*> allNPCs;
					std::vector<Npc*> possNPCs;

					// cast all npcs to the NPC class (they are stored as characters)
					for (int k = 0; k < GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs().size(); ++k) {
						allNPCs.push_back((Npc*)(GameManager::getSingletonPtr()->getLevelManager()->getFriendlyNpcs()[k]));
					}

					// find all relevant npcs
					for (int k = 0; k < allNPCs.size(); ++k) {
						// if this npc doesn't live in the start town
						if(pSourceNpc->getHomeTown()->id != allNPCs[k]->getHomeTown()->id) {
							possNPCs.push_back(allNPCs[k]);
						}
					}

					// do a random roll to chose a npc
					int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, possNPCs.size());
					randomNpc = possNPCs[randomroll];

					contentIDs.push_back(std::pair<IQuestContent*, int>(randomNpc, friendlies));
					friendlies++;
				}

				break;
			case EnemyQC:
				// pick a random enemy that hasn't been chosen yet.

				break;
			case TownQC:


				break;
			case HideOutQC:
				break;
			case QuestItemQC:
				break;
			default:
				Debug("Unknown quest item type occured while generating concrete quest.");
				break;
			}
		}



		// dialog
		thisAction->_concreteContent = concreteActionContent;
	}

	// rewards / maybe in abstract

	return pQuest;
}



