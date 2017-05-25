#ifndef NPC_H_
#define NPC_H_

#include "BaseNpc.h"
#include <fstream>
#include "Needs.h"
#include "Quest.h"

enum Profession {
	Smith = 0,
	Guard,
	Priest,
	Alchemist,
	Shopkeeper,
	Nobleman,

	AMOUNT_OF_PROFS
};

class Npc : public BaseNpc
{
public:
	Npc(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*, Building*);
	~Npc();

	bool getInDialog() { return _inDialog; }

	void update(Ogre::Real) override;
	bool talk(Ogre::Vector3);
	void die() override;

private: 
	bool _inDialog;					//!< True if the player is currently talking with this NPC.
	int _dialogCount;				//!< The amount of different parts the dialog consists of.
	std::vector<std::string> _dialog;

	std::string _name;
	NeedSet _needs;				//!< A set of needs, when the value of a need is low, this NPC wants something.
	Profession _profession;			//!< The profession of the NPC, used to generate relevant quests.
	City* _hometown;
	Building* _home;

	Quest* _currentQuest;
	bool _hasQuest;

	void adjustNeed(NeedType, int);
	void needNewQuest();

	std::vector<std::string> getNameOptions();
};

#endif