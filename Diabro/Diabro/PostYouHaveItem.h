#ifndef POST_YOU_HAVE_ITEM
#define POST_YOU_HAVE_ITEM

#include "PostCondition.h"
#include "IQuestContent.h"

class PostYouHaveItem : public PostCondition {
public:
	PostYouHaveItem() {}
	~PostYouHaveItem() {}

	void update() override;
};


#endif