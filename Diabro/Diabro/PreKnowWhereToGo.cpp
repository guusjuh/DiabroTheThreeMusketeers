#include "PreKnowWhereToGo.h"
#include "Debug.h"

void PreKnowWhereToGo::start() {
	city->setRelevantForAction(true);
	
	_isMet = true;

	Debug("Pre - know where to go - done");
}
