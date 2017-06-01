#ifndef POST_YOU_THERE
#define POST_YOU_THERE

#include "PostCondition.h"
#include "IQuestContent.h"

class PostYouThere : public PostCondition {
public:
	PostYouThere() {}
	~PostYouThere() {}

	void update() override;
};


#endif