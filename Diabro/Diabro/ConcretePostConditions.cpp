#include "ConcretePostConditions.h"
#include "Debug.h"
#include "Action.h"

void PostYouThere::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i].second == Action::msgCityReached) {
				_isMet = true;
				Debug("Post - you there - completed");
			}
		}
		queuedMsgs.clear();
	}
}

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

void PostYouInfo::update() {
	if (queuedMsgs.size() > 0) {
		for (int i = 0; i < queuedMsgs.size(); ++i) {
			if (queuedMsgs[i].second == Action::msgPlayerInfo) {
				_isMet = true;
				Debug("Post - you info - completed");
			}
		}
		queuedMsgs.clear();
	}
}

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


