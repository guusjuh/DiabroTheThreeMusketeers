#ifndef POST_THEY_HAVE_ITEM
#define POST_THEY_HAVE_ITEM

#include "PostCondition.h"
#include "IQuestContent.h"

class PostTheyHaveItem : public PostCondition {
public:
	PostTheyHaveItem() {}
	~PostTheyHaveItem() {}

	void update() override;
};


#endif