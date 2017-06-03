#include "Action.h"
#include "Debug.h"
#include "PreSomethingThere.h"
#include "GameManager.h"
#include "PostYouHaveItem.h"
#include "PreKnowWhereToGo.h"
#include "PostYouThere.h"
#include "PreSomebodyThere.h"
#include "PreYouItemOfInterest.h"
#include "PostTheyHaveItem.h"
#include "PostYouInfo.h"
#include "PostTheyInfo.h"
#include "PostTheyDead.h"

const std::string Action::msgCityReached = "City reached";
const std::string Action::msgPlayerItem = "Player received item";
const std::string Action::msgNpcItem = "Npc received item";
const std::string Action::msgPlayerInfo = "Player has info";
const std::string Action::msgNpcInfo = "Npc has info";
const std::string Action::msgEnemyDead = "Enemy dead";

/// <summary>
/// Initializes a new instance of the <see cref="Action"/> class.
/// </summary>
Action::Action() : _id(0), _type((ActionType)0), _preconditions(), _postcondition(), _requiredContent(), _concreteContent(0) {
	_completed = false;
}

/// <summary>
/// Initializes a new abstract instance of the <see cref="Action"/> class.
/// </summary>
/// <param name="pID">The id.</param>
/// <param name="pType">Type of the action.</param>
/// <param name="pPreconditions">The preconditions.</param>
/// <param name="pPostcondition">The postcondition.</param>
/// <param name="pQuestContent">Required content for this action.</param>
Action::Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::vector<QuestContent> pQuestContent, std::string dialog) :
	_id(pID), _type(pType), _concreteContent(0), _dialog(dialog) {
	std::vector<std::pair<QuestContent, int>>	tempRequiredContent;
	createPreConditions(pPreconditions);
	createPostConditions(pPostcondition);

	for (int i = 0; i < pQuestContent.size(); ++i) {
		tempRequiredContent.push_back(std::pair<QuestContent, int>(pQuestContent[i], 0));
	}

	_requiredContent = tempRequiredContent;

	_completed = false;
}

/// Initializes a new abstract instance of the <see cref="Action"/> class.
/// </summary>
/// <param name="pID">The id.</param>
/// <param name="pType">Type of the action.</param>
/// <param name="pPreconditions">The preconditions.</param>
/// <param name="pPostcondition">The postcondition.</param>
/// <param name="pQuestContent">Required content for this action.</param>
Action::Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, std::vector<std::pair<QuestContent, int>> pQuestContent, std::string dialog) :
	_id(pID), _type(pType), _requiredContent(pQuestContent), _concreteContent(0), _dialog(dialog) {
	createPreConditions(pPreconditions);
	createPostConditions(pPostcondition);

	_completed = false;
}

/// <summary>
/// Finalizes an instance of the <see cref="Action"/> class.
/// </summary>
Action::~Action() {}

void Action::start() {
	// make pre conditions happen
	std::map<PreconditionsType, PreCondition*>::iterator it;
	for (it = _preconditions.begin(); it != _preconditions.end(); it++) {
		if(it->second == nullptr) continue;
		it->second->start();
	}

	// check for pre conditions being true
}

void Action::update() {
	// update postcondition
	if (_postcondition.second == nullptr) return;

	_postcondition.second->update();

	// check for condition being met
	if(_postcondition.second->isMet()) {
 		complete();
		GameManager::getSingletonPtr()->getSoundManager()->completed();
	}
}

void Action::end() {
	// finalize the action.
	for (int i = 0; i < _concreteContent.size(); ++i) {
		if (_concreteContent[i].first == nullptr) 
			return;
		_concreteContent[i].first->setRelevantForAction(false);
	}
}

std::vector<QuestContent> Action::getRequiredContentTypes() {
	std::vector<QuestContent> returnVector;
	for (int i = 0; i < getRequiredContent().size(); ++i) {
		returnVector.push_back(_requiredContent[i].first);
	}
	return returnVector;
}

IQuestContent* Action::getTarget() {
	int idTarget = -1;

	idTarget = contentContains(NPCQC);
	if(idTarget < 0) {
		idTarget = contentContains(EnemyQC);
		if (idTarget < 0) {
			idTarget = contentContains(TownQC);
			if (idTarget < 0) {
				idTarget = contentContains(HideOutQC);
				if (idTarget < 0) {
					// something wrong.
				}
			}
		}
	}

	return _concreteContent[idTarget].first;
}

int Action::contentContains(QuestContent type) {
	for (int i = 0; i < _concreteContent.size(); ++i) {
		if(_concreteContent[i].first->getType() == type) {
			return i;
		}
	}

	return -1;
}

void Action::createPreConditions(std::vector<PreconditionsType> precontypes) {
	for (int i = 0; i < precontypes.size(); ++i) {
		PreCondition* precondition = nullptr;

		switch(precontypes[i]) {
		case SomebodyThere:
			precondition = new PreSomebodyThere();
			break;
		case SomethingThere:
			precondition = new PreSomethingThere();
			break;
		case KnowWhereToGo:
			precondition = new PreKnowWhereToGo();
			break;
		case YouItemOfInterest:
			precondition = new PreYouItemOfInterest();
			break;
		}

		_preconditions[precontypes[i]] = precondition;
	}
}

void Action::createPostConditions(PostconditionType postType) {
	PostCondition* postcondition = nullptr;

	switch (postType) {
	case TheyHaveTheItem:
		postcondition = new PostTheyHaveItem();
		break;
	case YouHaveTheItem:
		postcondition = new PostYouHaveItem();
		break;
	case YouThere:
		postcondition = new PostYouThere();
		break;
	case YouInfo:
		postcondition = new PostYouInfo();
		break;
	case TheyInfo:
		postcondition = new PostTheyInfo();
		break;
	case TheyDead:
		postcondition = new PostTheyDead();
		break;
	}

	_postcondition = std::pair<PostconditionType, PostCondition*>(postType, postcondition);
}

void Action::setPreConditionsContent() {
	std::map<PreconditionsType, PreCondition*>::iterator it;
	for (it = _preconditions.begin(); it != _preconditions.end(); it++) {
		switch (it->first) {
		case SomebodyThere:
			for (int j = 0; j < _concreteContent.size(); ++j) {
				if (_concreteContent[j].first->getType() == EnemyQC) {
					if(_concreteContent[j].first != nullptr) {
						((PreSomebodyThere*)it->second)->characters.push_back(_concreteContent[j].first);
					}
					else {
						BasicEnemy* randomEnemy;
						std::vector<BasicEnemy*> allEnemies;

						// cast all npcs to the NPC class (they are stored as characters)
						for (int k = 0; k < GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs().size(); ++k) {
							allEnemies.push_back((BasicEnemy*)(GameManager::getSingletonPtr()->getLevelManager()->getHostileNpcs()[k]));
						}

						// do a random roll to chose an enemy
						int randomroll = GameManager::getSingletonPtr()->getRandomInRange(0, allEnemies.size());
						randomEnemy = allEnemies[randomroll];
						((PreSomebodyThere*)it->second)->characters.push_back(randomEnemy);
					}
				}
				if(_concreteContent[j].first->getType() == NPCQC) {
					((PreSomebodyThere*)it->second)->characters.push_back(_concreteContent[j].first);
				}
				if (_concreteContent[j].first->getType() == TownQC || _concreteContent[j].first->getType() == HideOutQC) {
					((PreSomebodyThere*)it->second)->city = _concreteContent[j].first;
				}
			}
			break;
		case SomethingThere:
			for (int j = 0; j < _concreteContent.size(); ++j) {
				if (_concreteContent[j].first->getType() == EnemyQC || _concreteContent[j].first->getType() == NPCQC) {
					((PreSomethingThere*)it->second)->character = _concreteContent[j].first;
				}
				if (_concreteContent[j].first->getType() == QuestItemQC) {
					((PreSomethingThere*)it->second)->item = _concreteContent[j].first;
				}
			}

			break;
		case KnowWhereToGo:
			for (int j = 0; j < _concreteContent.size(); ++j) {
				if (_concreteContent[j].first->getType() == TownQC) {
					((PreKnowWhereToGo*)it->second)->city = _concreteContent[j].first;
				}
			}			
			break;
		case YouItemOfInterest:
			for (int j = 0; j < _concreteContent.size(); ++j) {
				if (_concreteContent[j].first->getType() == QuestItemQC) {
					((PreYouItemOfInterest*)it->second)->item = _concreteContent[j].first;
				}
			}			
			break;
		default:
			break;
		}
	}
	
}

void Action::setPostConditionsContent() {
	
}

void Action::sendMsg(std::string msg) {
	if(_postcondition.second != nullptr) {
		_postcondition.second->receiveMsg(msg);
	}
}
