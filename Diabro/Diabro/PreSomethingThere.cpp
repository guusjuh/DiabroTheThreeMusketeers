#include "PreSomethingThere.h"
#include "City.h"
#include "Debug.h"
#include "Npc.h"
#include "BasicEnemy.h"

void PreSomethingThere::start() {
	// give the found item to the character

	if(character->getType() == NPCQC) {
		((Npc*)character)->recieveItem();

	} else {
		((BasicEnemy*)character)->recieveItem();
	}

	character->setRelevantForAction(true);
	item->setRelevantForAction(true);

	_isMet = true;
	Debug("Pre - something there - done");
}
