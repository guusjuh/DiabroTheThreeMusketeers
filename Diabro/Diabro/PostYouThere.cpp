#include "PostYouThere.h"
#include "Debug.h"
#include "Action.h"

void PostYouThere::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i] == Action::msgCityReached) {
				_isMet = true;
				Debug("Post - you there - completed");
			}
		}
		queuedMsgs.clear();
	}
}

