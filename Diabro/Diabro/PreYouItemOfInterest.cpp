#include "PreYouItemOfInterest.h"
#include "City.h"
#include "Debug.h"
#include "Npc.h"
#include "BasicEnemy.h"
#include "GameManager.h"

void PreYouItemOfInterest::start() {
	if(!GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->hasItem()) {
		Debug("Error occured: player managed to get here without getting an item..");
		return;
	}

	item->setRelevantForAction(true);

	_isMet = true;
	Debug("Pre - you have item of interest - done");
}
