#include "PostTheyHaveItem.h"
#include "Debug.h"
#include "Action.h"

void PostTheyHaveItem::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i].second == Action::msgNpcItem) {
				_isMet = true;
				Debug("Post - they have item - completed");
			}
		}
		queuedMsgs.clear();
	}
}

