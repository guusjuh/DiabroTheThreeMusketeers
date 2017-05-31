#ifndef I_QUEST_CONTENT_H
#define I_QUEST_CONTENT_H
#include <OgreEntity.h>

/// <summary>
/// Enum for the types of content used in a quest. 
/// </summary>
enum QuestContent {
	QuestItemQC = 0,
	TownQC,
	HideOutQC,
	NPCQC,
	EnemyQC
};

/// <summary>
/// An interface to ensure the content can be used in a quest. 
/// </summary>
class IQuestContent {
public:
	IQuestContent() { _relevantForAction = false; }
	~IQuestContent(){}

	virtual QuestContent getType() { return (QuestContent)0; }
	virtual Ogre::Vector3 getQuestPosition() { return Ogre::Vector3(0,0,0); }

	/// <summary>
	/// IF YOU TOUCH THIS U DEAD
	/// </summary>
	void setRelevantForAction(bool val) {
		_relevantForAction = val;
	}

protected:
	bool _relevantForAction;
	
	void sendMsg(std::string msg);
};

#endif