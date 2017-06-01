#include "PreSomethingThere.h"
#include "City.h"
#include "Debug.h"
#include "Npc.h"
#include "BasicEnemy.h"

void PreSomethingThere::start() {
	// give the found item to the character

	if(character->getType() == NPCQC) {
		((Npc*)character)->recieveItem();
		((Npc*)character)->needToGiveItem();
	} else {
		((BasicEnemy*)character)->recieveItem();
		((BasicEnemy*)character)->needToGiveItem();
	}

	character->setRelevantForAction(true);
	item->setRelevantForAction(true);

	_isMet = true;
	Debug("Pre - something there - done");
}
