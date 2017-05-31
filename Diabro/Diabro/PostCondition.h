#ifndef POST_CONDITION_H
#define POST_CONDITION_H
#include <vector>

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

	void receiveMsg(std::string msg) { queuedMsgs.push_back(msg); }

protected:
	std::vector<std::string> queuedMsgs;
	bool _isMet;
};

#endif
