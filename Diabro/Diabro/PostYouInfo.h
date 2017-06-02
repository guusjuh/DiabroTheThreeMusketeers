#ifndef POST_YOU_INFO
#define POST_YOU_INFO

#include "PostCondition.h"

class PostYouInfo : public PostCondition {
public:
	PostYouInfo() {}
	~PostYouInfo() {}

	void update() override;
};

#endif