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

/// <summary>
/// A strategy forms the basis for a quest by defining it's actions and content.
/// </summary>
class Strategy {
public:
	/// <summary>
	/// Initializes a new instance of the <see cref="Strategy"/> class.
	/// </summary>
	/// <param name="pid">The id of the strategy.</param>
	/// <param name="pName">Name of the strategy.</param>
	/// <param name="pNameQuest">The template name of the quest.</param>
	/// <param name="pMotivation">The motivation for the quest.</param>
	/// <param name="pActions">The actions that must be completed for the quest.</param>
	Strategy(int pid, std::string pName, QuestName pNameQuest, NeedType pMotivation, std::vector<Action*> pActions)
		: _id(pid), _nameStrategy(pName), _nameQuest(pNameQuest), _motivation(pMotivation), _actionSequence(pActions) { }

	/// <summary>
	/// Initializes a new instance of the <see cref="Strategy"/> class.
	/// </summary>
	Strategy() {}

	/// <summary>
	/// Finalizes an instance of the <see cref="Strategy"/> class.
	/// </summary>
	~Strategy() {
		for (int i = 0; i < _actionSequence.size(); ++i) {
			if (_actionSequence[i] != nullptr) {
				delete _actionSequence[i];
			}
		}
	}

	int getID() { return _id; }
	std::string getName() { return _nameStrategy; }
	QuestName getNameQuest() { return _nameQuest; }
	NeedType getMotivation() { return _motivation; }
	std::vector<Action*> getActionSequence() { return _actionSequence; }

private:
	int _id;
	std::string _nameStrategy;
	QuestName _nameQuest;
	NeedType _motivation;
	std::vector<Action*> _actionSequence;
};

#endif 
