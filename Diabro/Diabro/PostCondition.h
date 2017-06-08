#ifndef POST_CONDITION_H
#define POST_CONDITION_H
#include <vector>
#include <string>
#include "IQuestContent.h"
//TODO: implement
// 1. you have the item
// 2. they have the item
// 3. you there

class PostCondition {
public:
	//TODO: moet ismet overal op false/base constructor aanroepen?
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
