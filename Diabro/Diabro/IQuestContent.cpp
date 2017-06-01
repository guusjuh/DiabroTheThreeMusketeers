#include "IQuestContent.h"
#include "GameManager.h"

void IQuestContent::sendMsg(std::string msg) {
	if (_relevantForAction) {
		GameManager::getSingletonPtr()->getQuestManager()->getCurrentQuest();
	}
}

