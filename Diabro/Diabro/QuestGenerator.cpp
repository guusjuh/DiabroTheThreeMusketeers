#include "QuestGenerator.h"
#include "GameManager.h"
#include "Debug.h"
#include <regex>

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
			tempActions[i].getPostcondition(), tempActions[i].getRequiredContent(), tempActions[i].getDialog()));
	}

	Strategy choosenStrat = Strategy(possibleStrategies[randomRoll]->getID(), possibleStrategies[randomRoll]->getName(), 
		possibleStrategies[randomRoll]->getNameQuest(), possibleStrategies[randomRoll]->getDialog(), possibleStrategies[randomRoll]->getMotivation(),
		newActions, possibleStrategies[randomRoll]->getRarityPref());

	float healthReward = (GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->getMaxHealth() / 100) * 10;
	PlayerUpgradeType upgradeReward = generateRandomUpgrade(); 

	// assign values to the quest
	pQuest = new Quest(choosenStrat, pSourceNpc, healthReward, upgradeReward);

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
	int friendlies = 1;
	int enemies = 1;
	int cities = 1;
	int hideouts = 1;
	int items = 1;

	contentIDs.push_back(std::pair<IQuestContent*, int>(pSourceNpc, friendlies));
	friendlies++;

	contentIDs.push_back(std::pair<IQuestContent*, int>(pSourceNpc->getHomeTown(), cities));
	cities++;

	// for each action
	std::vector<Action> actions = pQuest->_strategy.getActionSequence();
	for (int i = 0; i < actions.size(); ++i) {
		std::vector<std::pair<IQuestContent*, int>> concreteActionContent;
		Action& thisAction = actions[i];

		// for each content required by the action
		for (int j = 0; j < thisAction._requiredContent.size(); ++j) {
			QuestContent type = thisAction._requiredContent[j].first;
			int id = thisAction._requiredContent[j].second;

			// switch on the given item type
			switch (type) {
			case NPCQC:
				// if this id is already used in this quest
				if (id < friendlies) {
					concreteActionContent.push_back(findTypeAndID(contentIDs, NPCQC, id));
				}
				// else, a new npc has to be found is a different city
				else {
					//TODO: prefer other npcs based on their profession and the current strategy
					Npc* chosenNPC = getRandomNPC(pSourceNpc);
					concreteActionContent.push_back(std::pair<IQuestContent*, int>(chosenNPC, friendlies));
					contentIDs.push_back(std::pair<IQuestContent*, int>(chosenNPC, friendlies));
					friendlies++;

					// check for an hideout already being chosen so the NPC can be placed there. 
					if (j > 0) {
						for (int k = 0; k < j; ++k) {
							if (thisAction._requiredContent[k].first == HideOutQC) {
								//TODO: actually place the NPC there and set its fsm accordingly
							}
						}
					}
				}

				break;

				// pick a random enemy that hasn't been chosen yet.
			case EnemyQC:
				// if this id is already used in this quest
				if (id < enemies) {
					concreteActionContent.push_back(findTypeAndID(contentIDs, EnemyQC, id));
				}
				// else, a new npc has to be found is a different city
				else {
					BasicEnemy* chosenEnemy = getRandomEnemy();
					concreteActionContent.push_back(std::pair<IQuestContent*, int>(chosenEnemy, enemies));
					contentIDs.push_back(std::pair<IQuestContent*, int>(chosenEnemy, enemies));
					enemies++;
				}

				break;
			case TownQC:
				// if this id is already used in this quest
				if (id < cities) {
					concreteActionContent.push_back(findTypeAndID(contentIDs, TownQC, id));
				}
				// else, a new npc has to be found is a different city
				else {
					City* chosenCity = nullptr;

					// check for a npc already chosen. 
					if (j > 0) {
						for (int k = 0; k < j; ++k) {
							if (thisAction._requiredContent[k].first == NPCQC) {
								Npc* temp = (Npc*)concreteActionContent[k].first;
								chosenCity = temp->getHomeTown();
								concreteActionContent.push_back(std::pair<IQuestContent*, int>(chosenCity, cities));
								contentIDs.push_back(std::pair<IQuestContent*, int>(chosenCity, cities));
								cities++;
								break;
							}
						}

						if (chosenCity == nullptr) break;
					}


					//TODO: optimize this.
					int notWantedCityID = pSourceNpc->getHomeTown()->id;

					if(i != 0) {
						for (int k = 0; k < actions[i - 1]._concreteContent.size(); ++k) {
							if (actions[i - 1]._concreteContent[k].first->getType() == NPCQC || actions[i - 1]._concreteContent[k].first->getType() == TownQC) {
								notWantedCityID = actions[i - 1]._concreteContent[k].first->getType() == NPCQC ? ((Npc*)(actions[i - 1]._concreteContent[k].first))->getHomeTown()->id
									: ((City*)actions[i - 1]._concreteContent[k].first)->id;
							}
						}
					}


					do{
						chosenCity = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZonePointer(0, 0)->getRandomCity(CityRT);
					} while (chosenCity->id == notWantedCityID);

					concreteActionContent.push_back(std::pair<IQuestContent*, int>(chosenCity, cities));
					contentIDs.push_back(std::pair<IQuestContent*, int>(chosenCity, cities));

					cities++;
				}

				break;
			case HideOutQC:
				// if this id is already used in this quest
				if (id < hideouts) {
					concreteActionContent.push_back(findTypeAndID(contentIDs, HideOutQC, id));
				}
				// else, a new npc has to be found is a different city
				else {
					City* chosenCity = nullptr;

					// check for a npc already chosen. 
					if (j > 0) {
						for (int k = 0; k < j; ++k) {
							if (thisAction._requiredContent[k].first == EnemyQC) {
								BasicEnemy* temp = (BasicEnemy*)concreteActionContent[k].first;
								chosenCity = temp->getMyCity();
								concreteActionContent.push_back(std::pair<IQuestContent*, int>(chosenCity, hideouts));
								contentIDs.push_back(std::pair<IQuestContent*, int>(chosenCity, hideouts));
								hideouts++;
								break;
							}
						}

						if (chosenCity == nullptr) break;
					}


					//TODO: optimize this.
					int notWantedCityID = -1;//pSourceNpc->getHomeTown()->id;

					if (i != 0) {
						for (int k = 0; k < actions[i - 1]._concreteContent.size(); ++k) {
							if (actions[i - 1]._concreteContent[k].first->getType() == EnemyQC || actions[i - 1]._concreteContent[k].first->getType() == HideOutQC) {
								notWantedCityID = actions[i - 1]._concreteContent[k].first->getType() == EnemyQC ? ((BasicEnemy*)(actions[i - 1]._concreteContent[k].first))->getMyCity()->id
									: ((City*)actions[i - 1]._concreteContent[k].first)->id;
							}
						}
					}

					do {
						chosenCity = GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZonePointer(0, 0)->getRandomCity(HideoutRT);
					} while (chosenCity->id == notWantedCityID);

					concreteActionContent.push_back(std::pair<IQuestContent*, int>(chosenCity, hideouts));
					contentIDs.push_back(std::pair<IQuestContent*, int>(chosenCity, hideouts));

					hideouts++;
				}

				break;
			case QuestItemQC:
				if (id < items) {
					concreteActionContent.push_back(findTypeAndID(contentIDs, QuestItemQC, id));
				}
				else {
					QuestItem* item = GameManager::getSingletonPtr()->getQuestManager()->getQuestContentManager()->getItemGenerator()->generateItem(pQuest->_strategy.getRarityPref());
					concreteActionContent.push_back(std::pair<IQuestContent*, int>(item, items));
					contentIDs.push_back(std::pair<IQuestContent*, int>(item, items));
					items++;
				}

				break;
			default:
				Debug("Unknown quest item type occured while generating concrete quest.");
				break;
			}
		}

		// dialog
		thisAction._concreteContent = concreteActionContent;
		thisAction.setPreConditionsContent();
	}

	// generate quest name based on chosen content
	pQuest->_strategy._nameQuest = getFilledTemplate(pQuest->_strategy._nameQuest, contentIDs);

	// generate start dialog based on chosen content
	pQuest->_strategy._startDialog = getFilledTemplate(pQuest->_strategy._startDialog, contentIDs);

	// generate dialog for all actions
	for (int i = 0; i < actions.size(); ++i) {
		actions[i]._dialog = getFilledTemplate(actions[i]._dialog, actions[i]._concreteContent);
	}

	// set the action sequence
	pQuest->_strategy._actionSequence = actions;

	return pQuest;
}

Npc* QuestGenerator::getRandomNPC(Npc* pSourceNpc) {
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
		if (pSourceNpc->getHomeTown()->id != allNPCs[k]->getHomeTown()->id) {
			possNPCs.push_back(allNPCs[k]);
		}
	}

	// do a random roll to chose a npc
	int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, possNPCs.size());
	randomNpc = possNPCs[randomroll];

	return randomNpc;
}

BasicEnemy* QuestGenerator::getRandomEnemy() {
	BasicEnemy* randomEnemy;
	std::vector<BasicEnemy*> allEnemies;

	// cast all npcs to the NPC class (they are stored as characters)
	for (int k = 0; k < GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs().size(); ++k) {
		allEnemies.push_back((BasicEnemy*)(GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs()[k]));
	}

	// do a random roll to chose an enemy
	int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, allEnemies.size());
	randomEnemy = allEnemies[randomroll];

	return randomEnemy;
}

City* QuestGenerator::getRandomCity(RoomType type) {
	std::vector<City*> possCities;

	for(int i = 0; i < GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities.size(); ++i) {
		if(GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i].typeFlag == type) {
			possCities.push_back(&GameManager::getSingletonPtr()->getLevelManager()->levelGenerator->getZone(0, 0).cities[i]);
		}
	}

	// do a random roll to chose a npc
	int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, possCities.size());
	return possCities[randomroll];
}

std::pair<IQuestContent*, int> QuestGenerator::findTypeAndID(std::vector<std::pair<IQuestContent*, int>> searchSpace, QuestContent type, int id) {
	for (int k = 0; k < searchSpace.size(); ++k) {
		IQuestContent* temp = searchSpace[k].first;
		if (searchSpace[k].first->getType() == type && searchSpace[k].second == id) {
			return searchSpace[k];
		}
	}
}

std::string QuestGenerator::getFilledTemplate(std::string templateString, std::vector<std::pair<IQuestContent*, int>> concreteContent) {
	// info to replace templates from string
	QuestContent contentType;
	std::pair<IQuestContent*, int> thisTemplate;

	std::string toReturn = "";
	int id;
	std::string typeString;

	std::regex expr("\'([A-Z||a-z]+)([0-9]+)\'([^ ]*)");	 // the template to match
	std::smatch match;										 // the matches to be found

	while (std::regex_search(templateString, match, expr)) {
		// add everything before the found pattern to the return string
		toReturn += match.prefix().str();

		typeString = match[1].str();
		id = atoi(match[2].str().c_str());

		// find the enums for all the type strings found
		for (std::map<std::string, QuestContent>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.begin();
			it != GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.end(); ++it) {
			if (typeString == it->first) {
				contentType = it->second;
			}
		}

		thisTemplate = findTypeAndID(concreteContent, contentType, id);

		std::string replaceString = "";
		switch (thisTemplate.first->getType()) {
		case NPCQC:
			replaceString = ((Npc*)thisTemplate.first)->_name;
			break;
		case EnemyQC:
			replaceString = ((BasicEnemy*)thisTemplate.first)->getName();
			break;
		case TownQC:
			replaceString = ((City*)thisTemplate.first)->getName();
			break;
		case HideOutQC:
			replaceString = ((City*)thisTemplate.first)->getName();
			break;
		case QuestItemQC:
			replaceString = ((QuestItem*)thisTemplate.first)->getName();
			break;
		}

		toReturn += replaceString;
		
		templateString = match.suffix().str();
	}

	if(templateString != "") {
		toReturn += templateString;
	}

	return toReturn;
}

PlayerUpgradeType QuestGenerator::generateRandomUpgrade() {
	UpgradeModifierType type = (UpgradeModifierType)GameManager::getSingletonPtr()->getRandomInRange(1, AMOUNT_OF_UPGRADE_TYPES - 1);

	int low = 0;
	int high = 1;
	int value = low;

	switch (type) {
	case Health:
		low = Player::LOW_HP;
		high = Player::HIGH_HP;
		break;
	case Damage:
		low = Player::LOW_DMG;
		high = Player::HIGH_DMG;
		break;
	default:
		break;
	}

	// random value between low and high
	value = GameManager::getSingletonPtr()->getRandomInRange(low, high);

	return PlayerUpgradeType(value, type);
}
