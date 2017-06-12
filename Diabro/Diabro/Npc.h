#ifndef NPC_H_
#define NPC_H_

#include "BaseNpc.h"
#include <fstream>
#include "Needs.h"
#include "Quest.h"
#include "NpcWanderState.h"
#include "NpcFollowPlayerState.h"
#include "NpcIdleState.h"
#include "NpcKidnappedState.h"

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
	friend class DialogManager;
	friend class Quest;

public:
	Npc(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*, Building*);
	~Npc();

	bool getInDialog() { return _inDialog; }

	void update(Ogre::Real) override;
	void die() override;
	void collide() override;

	City* getHomeTown() { return _hometown; }
	std::string getName() { return _name; }

	QuestContent getType() override { return NPCQC; }
	bool talk() override;

	bool isKidnapped() { return _kidnapped; }
	void setKidnapped(bool val) { _kidnapped = val; }

private: 
	bool _kidnapped;

	bool _initialized;
	bool _hasQuest;
	Quest* _currentQuest;

	std::string _name;
	NeedSet _needs;					//!< A set of needs, when the value of a need is low, this NPC wants something.
	Profession _profession;			//!< The profession of the NPC, used to generate relevant quests.
	City* _hometown;
	Building* _home;

	void adjustNeed(NeedType, int);
	void needNewQuest();

	std::string getRandomName();
	static const std::string _nameOptions[];
	static std::vector<std::string> _usedNameOptions;

	void setRelevantForAction(bool val) override{
		// don't change if u don't have to
		if (_relevantForAction == val) return;

		IQuestContent::setRelevantForAction(val);

		// if you're relevant, set the indicator on with correct color
		if (_relevantForAction) {
			questIndicatorEntity->setMaterialName(_activeMaterial);
			indicatorNode->setVisible(true);
		}
		else {
			if(_hasQuest) {
				questIndicatorEntity->setMaterialName(_questMaterial);
				indicatorNode->setVisible(true);
			} else {
				indicatorNode->setVisible(false);
			}
		}
	}
};

#endif