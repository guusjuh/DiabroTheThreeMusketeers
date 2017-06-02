#ifndef POST_THEY_DEAD
#define POST_THEY_DEAD

#include "PostCondition.h"

class PostTheyDead : public PostCondition {
public:
	PostTheyDead() {}
	~PostTheyDead() {}

	void update() override;
};

#endif