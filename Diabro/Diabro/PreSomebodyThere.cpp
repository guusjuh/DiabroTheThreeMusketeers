#include "PreSomebodyThere.h"
#include "City.h"
#include "Debug.h"
#include "Npc.h"
#include "BasicEnemy.h"

void PreSomebodyThere::start() {
	for (int i = 0; i < characters.size(); ++i) {
		if (characters[i]->getType() == NPCQC) {
			if (!((City*)city)->inThisCity(characters[i]->getQuestPosition())) {
				//TODO: move character to city
				Debug("Error occurred: no npc in the city");
				return;
			}
		}
		else if(characters[i]->getType() == EnemyQC){
			// its an enemy
			if (!((City*)city)->inThisCity(characters[i]->getQuestPosition())) {
				Debug("Error occurred: no enemy in the city");
				return;
			}
		}		
		characters[i]->setRelevantForAction(true);
	}

	_isMet = true;
	Debug("Pre - somebody there - done");
}
