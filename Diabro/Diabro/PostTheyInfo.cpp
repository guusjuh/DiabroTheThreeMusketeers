#include "PostTheyInfo.h"
#include "Debug.h"
#include "Action.h"

void PostTheyInfo::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i].second == Action::msgNpcInfo) {
				_isMet = true;
				Debug("Post - they info - completed");
			}
		}
		queuedMsgs.clear();
	}
}

