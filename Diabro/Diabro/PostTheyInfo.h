#ifndef POST_THEY_INFO
#define POST_THEY_INFO

#include "PostCondition.h"
#include "IQuestContent.h"

class PostTheyInfo : public PostCondition {
public:
	PostTheyInfo() {}
	~PostTheyInfo() {}

	void update() override;
};

#endif