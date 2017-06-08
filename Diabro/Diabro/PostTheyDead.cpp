#include "PostTheyDead.h"
#include "Debug.h"
#include "Action.h"

void PostTheyDead::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i].second == Action::msgEnemyDead) {
				_isMet = true;
				Debug("Post - they dead - completed");
			}
		}
		queuedMsgs.clear();
	}
}

