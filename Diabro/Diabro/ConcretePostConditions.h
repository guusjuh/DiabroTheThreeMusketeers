#ifndef POST_CONCRETE_PRE_CONDITIONS
#define POST_CONCRETE_PRE_CONDITIONS
#include "PostCondition.h"

class PostYouInfo : public PostCondition {
public:
	PostYouInfo() {}
	~PostYouInfo() {}

	void update() override;
};

class PostYouHaveItem : public PostCondition {
public:
	PostYouHaveItem() {}
	~PostYouHaveItem() {}

	void update() override;
};

class PostTheyInfo : public PostCondition {
public:
	PostTheyInfo() {}
	~PostTheyInfo() {}

	void update() override;
};

class PostTheyHaveItem : public PostCondition {
public:
	PostTheyHaveItem() {}
	~PostTheyHaveItem() {}

	void update() override;
};

class PostTheyDead : public PostCondition {
public:
	PostTheyDead() {}
	~PostTheyDead() {}

	void update() override;
};


class PostYouThere : public PostCondition {
public:
	PostYouThere() {}
	~PostYouThere() {}

	void update() override;
};

#endif