#ifndef POST_CONCRETE_PRE_CONDITIONS
#define POST_CONCRETE_PRE_CONDITIONS
#include "PostCondition.h"
#include "IQuestContent.h"

class PostYouInfo : public PostCondition {
	friend class Action;
public:
	PostYouInfo() {}
	~PostYouInfo() {}

	void update() override;

private:
	IQuestContent* npcWithInfo;
};

class PostYouHaveItem : public PostCondition {
	friend class Action;
public:
	PostYouHaveItem() {}
	~PostYouHaveItem() {}

	void update() override;
};

class PostYouThere : public PostCondition {
	friend class Action;
public:
	PostYouThere() {}
	~PostYouThere() {}

	void update() override;

private:
	IQuestContent* city;
};

class PostTheyInfo : public PostCondition {
	friend class Action;
public:
	PostTheyInfo() {}
	~PostTheyInfo() {}

	void update() override;
};

class PostTheyHaveItem : public PostCondition {
	friend class Action;
public:
	PostTheyHaveItem() {}
	~PostTheyHaveItem() {}

	void update() override;

private:
	IQuestContent* npc;
};

class PostTheyDead : public PostCondition {
	friend class Action;
public:
	PostTheyDead() {}
	~PostTheyDead() {}

	void update() override;

private:
	IQuestContent* enemy;
};

#endif