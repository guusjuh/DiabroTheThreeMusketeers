#include "QuestManager.h"
#include "ActionContainer.h"

/// <summary>
/// Initializes a new instance of the <see cref="QuestManager"/> class.
/// </summary>
QuestManager::QuestManager()
{
	stringToActionType.insert(std::pair<std::string, ActionType>("Escort", Escort));
	stringToActionType.insert(std::pair<std::string, ActionType>("Exchange", Exchange));
	stringToActionType.insert(std::pair<std::string, ActionType>("Explore", Explore));
	stringToActionType.insert(std::pair<std::string, ActionType>("Get", Get));
	stringToActionType.insert(std::pair<std::string, ActionType>("Give", Give));
	stringToActionType.insert(std::pair<std::string, ActionType>("Goto", Goto));
	stringToActionType.insert(std::pair<std::string, ActionType>("Kill", Kill));
	stringToActionType.insert(std::pair<std::string, ActionType>("Listen", Listen));
	stringToActionType.insert(std::pair<std::string, ActionType>("Report", Report));

	stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("SomebodyThere", SomebodyThere));
	stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("SomethingThere", SomethingThere));
	stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("KnowWhereToGo", KnowWhereToGo));
	stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("YouItemOfInterest", YouItemOfInterest));
	stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("TheyItemOfInterest", TheyItemOfInterest));
	stringToPreconditionType.insert(std::pair<std::string, PreconditionsType>("BothItemOfInterest", BothItemOfInterest));

	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("Accompanied", Accompanied));
	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("YouHaveTheItem", YouHaveTheItem));
	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("TheyHaveTheItem", TheyHaveTheItem));
	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("BothHaveTheItem", BothHaveTheItem));
	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("YouInfo", YouInfo));
	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("TheyInfo", TheyInfo));
	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("YouThere", YouThere));
	stringToPostconditionType.insert(std::pair<std::string, PostconditionType>("TheyDead", TheyDead));

	stringToQuestItemType.insert(std::pair<std::string, QuestItemType>("Book", Book));
	stringToQuestItemType.insert(std::pair<std::string, QuestItemType>("RareBook", RareBook));
	stringToQuestItemType.insert(std::pair<std::string, QuestItemType>("Food", Food));
	stringToQuestItemType.insert(std::pair<std::string, QuestItemType>("RawMaterial", RawMaterials));
	stringToQuestItemType.insert(std::pair<std::string, QuestItemType>("Gem", Gem));
	stringToQuestItemType.insert(std::pair<std::string, QuestItemType>("Mysterious", Mysterious));

	stringToQuestContentType.insert(std::pair<std::string, QuestContent>("QuestItemQC", QuestItemQC));
	stringToQuestContentType.insert(std::pair<std::string, QuestContent>("TownQC", TownQC));
	stringToQuestContentType.insert(std::pair<std::string, QuestContent>("HideOutQC", HideOutQC));
	stringToQuestContentType.insert(std::pair<std::string, QuestContent>("NPCQC", NPCQC));
	stringToQuestContentType.insert(std::pair<std::string, QuestContent>("EnemyQC", EnemyQC));

	stringToNeedType.insert(std::pair<std::string, NeedType>("Knowledge", KnowledgeNeed));
	stringToNeedType.insert(std::pair<std::string, NeedType>("Comfort", ComfortNeed));
	stringToNeedType.insert(std::pair<std::string, NeedType>("Reputation", ReputationNeed));
	stringToNeedType.insert(std::pair<std::string, NeedType>("Serenity", SerenityNeed));
	stringToNeedType.insert(std::pair<std::string, NeedType>("Protection", ProtectionNeed));
	stringToNeedType.insert(std::pair<std::string, NeedType>("Conquest", ConquestNeed));
	stringToNeedType.insert(std::pair<std::string, NeedType>("Wealth", WealthNeed));
	stringToNeedType.insert(std::pair<std::string, NeedType>("Equipment", EquipmentNeed));

}

/// <summary>
/// Generates a quest.
/// </summary>
/// <param name="pSourceNpc">The source NPC.</param>
/// <param name="pMotivation">The motivation.</param>
/// <returns></returns>
Quest* QuestManager::generateQuest(BaseNpc* pSourceNpc, NeedType pMotivation) {
	_quests.push_back(_questGenerator->generateQuest(pSourceNpc, pMotivation));

	return &_quests[_quests.size() - 1];
}
