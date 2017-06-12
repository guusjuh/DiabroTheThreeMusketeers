#include "ConcretePreConditions.h"
#include "Debug.h"
#include "City.h"
#include "Debug.h"
#include "Npc.h"
#include "BasicEnemy.h"
#include "GameManager.h"

void PreKnowWhereToGo::start() {
	city->setRelevantForAction(true);

	_isMet = true;

	Debug("Pre - know where to go - done");
}

void PreSomebodyThere::start() {
	for (int i = 0; i < characters.size(); ++i) {
		if (characters[i]->getType() == NPCQC) {
			if (!((City*)city)->inThisCity(characters[i]->getQuestPosition())) {
				//TODO: move character to city
				Debug("Error occurred: no npc in the city");
				return;
			}
		}
		else if (characters[i]->getType() == EnemyQC) {
			// its an enemy
/*			if (!((City*)city)->inThisCity(characters[i]->getQuestPosition())) {
				Debug("Warning occurred: no enemy in the city");
			}*/
		}
		characters[i]->setRelevantForAction(true);
	}

	_isMet = true;
	Debug("Pre - somebody there - done");
}

void PreSomethingThere::start() {
	// give the found item to the character

	if (character->getType() == NPCQC) {
		((Npc*)character)->recieveItem();
		((Npc*)character)->needToGiveItem();
	}
	else {
		((BasicEnemy*)character)->recieveItem();
		((BasicEnemy*)character)->needToGiveItem();
	}

	character->setRelevantForAction(true);
	item->setRelevantForAction(true);

	_isMet = true;
	Debug("Pre - something there - done");
}

void PreYouItemOfInterest::start() {
	if (!GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->hasItem()) {
		Debug("Error occured: player managed to get here without getting an item..");
		return;
	}

	GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->needToGiveItem();
	item->setRelevantForAction(true);

	_isMet = true;
	Debug("Pre - you have item of interest - done");
}