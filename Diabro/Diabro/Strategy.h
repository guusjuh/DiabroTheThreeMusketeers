#ifndef STRATEGY_H
#define STRATEGY_H

#pragma once
#include "Needs.h"
#include "Action.h"
#include "QuestContentManager.h"

struct QuestName {
	std::string preString;
	QuestContent templateContent;
	std::string postString;
};

class Strategy {
public:
	int GetID() { return _id; }

	std::string GetName() { return _nameStrategy; }
	std::string GetNameQuest() { return _nameQuest; }
	NeedType GetMotivation() { return _motivation; }
	std::vector<Action> GetActionSequence() { return _actionSequence; }

private:
	int _id;

	std::string _nameStrategy;
	std::string _nameQuest;

	NeedType _motivation;
	std::vector<Action> _actionSequence;
};

#endif STRATEGY_H
