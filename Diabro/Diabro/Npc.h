#ifndef NPC_H_
#define NPC_H_

#include "BaseNpc.h"
#include <fstream>
#include "Needs.h"
#include "Quest.h"

/// <summary>
/// Enum for the different professions of NPC's. An important variable for the quest system. 
/// </summary>
enum Profession {
	Smith = 0,
	Guard,
	Priest,
	Alchemist,
	Shopkeeper,
	Nobleman,

	AMOUNT_OF_PROFS
};

/// <summary>
/// The firendly NPC class. 
/// </summary>
/// <seealso cref="BaseNpc" />
class Npc : public BaseNpc
{
	friend class QuestGenerator;

public:
	Npc(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*, Building*);
	~Npc();

	bool getInDialog() { return _inDialog; }

	void update(Ogre::Real) override;
	bool talk(Ogre::Vector3);
	void die() override;

	City* getHomeTown() { return _hometown; }

	QuestContent getType() override { return NPCQC; }

private: 
	bool _hasQuest;
	Quest* _currentQuest;

	//TODO: if this npc is currently relevant for the action of the active quest, than don't trigger normal dialog text
	bool _inDialog;					//!< True if the player is currently talking with this NPC.
	int _dialogCount;				//!< The amount of different parts the dialog consists of.
	std::vector<std::string> _dialog;
	static const std::vector<std::string> getStndrtDialog() {
		std::vector<std::string> dialog;
		dialog.push_back("Hi there! How's it going fellow sphere?");
		dialog.push_back("Hm hm it sounds like you're on an existing journey.");
		dialog.push_back("Good luck and please take care of yourself. It can be dangerous around here.");

		return dialog;
	}

	void setDialog(std::string oneSentenceDialog) {
		_dialog.clear();
		_dialog.push_back(oneSentenceDialog);
	}

	std::string _name;
	NeedSet _needs;					//!< A set of needs, when the value of a need is low, this NPC wants something.
	Profession _profession;			//!< The profession of the NPC, used to generate relevant quests.
	City* _hometown;
	Building* _home;

	void adjustNeed(NeedType, int);
	void needNewQuest();

	std::vector<std::string> getNameOptions();
};

#endif