#include "PostYouHaveItem.h"
#include "Debug.h"
#include "Action.h"

void PostYouHaveItem::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i].second == Action::msgPlayerItem) {
				_isMet = true;
				Debug("Post - you have item - completed");
			}
		}
		queuedMsgs.clear();
	}
}

