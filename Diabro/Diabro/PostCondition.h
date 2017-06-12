#ifndef POST_CONDITION_H
#define POST_CONDITION_H
#include <vector>
#include <string>
#include "IQuestContent.h"

class PostCondition {
public:
	PostCondition() { _isMet = false; }
	~PostCondition() {}

	virtual void update();
	virtual void end() {}
	bool isMet() { return _isMet; }

	void receiveMsg(IQuestContent* sender, std::string msg) { queuedMsgs.push_back(std::pair<IQuestContent*, std::string>(sender, msg)); }

protected:
	std::vector<std::pair<IQuestContent*, std::string>> queuedMsgs;
	bool _isMet;
};

#endif
