#ifndef I_QUEST_CONTENT_H
#define I_QUEST_CONTENT_H

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
	IQuestContent(){}
	~IQuestContent(){}

	virtual QuestContent getType() { return (QuestContent)0; }
};

#endif