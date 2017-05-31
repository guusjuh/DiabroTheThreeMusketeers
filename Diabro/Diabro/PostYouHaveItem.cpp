#include "PostYouHaveItem.h"
#include "Debug.h"
#include "Action.h"

void PostYouHaveItem::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i] == Action::msgPlayerItem) {
				_isMet = true;
			}
		}
		queuedMsgs.clear();
	}
}

