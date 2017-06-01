#include "PostYouInfo.h"
#include "Debug.h"
#include "Action.h"

void PostYouInfo::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i] == Action::msgPlayerInfo) {
				_isMet = true;
				Debug("Post - you info - completed");
			}
		}
		queuedMsgs.clear();
	}
}

