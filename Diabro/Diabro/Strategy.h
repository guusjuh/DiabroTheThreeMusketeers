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
	Strategy(int pid, std::string pName, QuestName pNameQuest, NeedType pMotivation, std::vector<Action*> pActions)
		: _id(pid), _nameStrategy(pName), _nameQuest(pNameQuest), _motivation(pMotivation), _actionSequence(pActions)
	{
		
	}

	~Strategy() {
		for (int i = 0; i < _actionSequence.size(); ++i) {
			if (_actionSequence[i] != nullptr) {
				delete _actionSequence[i];
			}
		}
	}

	int GetID() { return _id; }

	std::string GetName() { return _nameStrategy; }
	QuestName GetNameQuest() { return _nameQuest; }
	NeedType GetMotivation() { return _motivation; }
	std::vector<Action*> GetActionSequence() { return _actionSequence; }

private:
	int _id;

	std::string _nameStrategy;
	QuestName _nameQuest;

	NeedType _motivation;
	std::vector<Action*> _actionSequence;
};

#endif 
