#include "PostCondition.h"
#include "Debug.h"
#include "Strategy.h"

void PostCondition::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i] == Action::msgPlayerItem) {
				Debug("Player claims to have an item");
			}
			else if (queuedMsgs[i] == Action::msgNpcItem) {
				Debug("Npc claims to have an item");

			}
			else if (queuedMsgs[i] == Action::msgCityReached) {
				Debug("Player reached city");

			}
		}
		queuedMsgs.clear();
	}
}

