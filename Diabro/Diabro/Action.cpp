#include "Action.h"
#include "Debug.h"
#include "GameManager.h"
#include "ConcretePostConditions.h"
#include "ConcretePreConditions.h"
#include <regex>
#include <string>

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
Action::Action(int pID, ActionType pType, std::vector<PreconditionsType> pPreconditions, PostconditionType pPostcondition, 
	std::vector<std::pair<QuestContent, int>> pQuestContent, std::string dialog, std::string instruction) :
	_id(pID), _type(pType), _requiredContent(pQuestContent), _concreteContent(0), _dialog(dialog), _instruction(instruction) {

	createPreConditions(pPreconditions);
	createPostConditions(pPostcondition);

	_completed = false;
}

/// <summary>
/// Finalizes an instance of the <see cref="Action"/> class.
/// </summary>
Action::~Action() {}

void Action::start() {
	setPreConditionsContent();
	setPostConditionsContent();

	// make pre conditions happen
	std::map<PreconditionsType, PreCondition*>::iterator it;
	for (it = _preconditions.begin(); it != _preconditions.end(); it++) {
		if(it->second == nullptr) continue;
		it->second->start();
	}
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

void Action::abandon() {
	// finalize the action.
	for (int i = 0; i < _concreteContent.size(); ++i) {
		if (_concreteContent[i].first == nullptr)
			return;
		_concreteContent[i].first->setRelevantForAction(false);

		// set variables regarding items so that next actions cannot be completed
		if (GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->hasItem()) {
			GameManager::getSingletonPtr()->getLevelManager()->getPlayer()->resetItem();
		}

		if (_concreteContent[i].first->getType() == EnemyQC) {
			if (((BasicEnemy*)_concreteContent[i].first)->hasItem()) {
				((BasicEnemy*)_concreteContent[i].first)->resetItem();
			}
		}
		else if (_concreteContent[i].first->getType() == NPCQC) {
			if (((Npc*)_concreteContent[i].first)->hasItem()) {
				((Npc*)_concreteContent[i].first)->resetItem();
			}
		}
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
					Debug("No target set for the quest");
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

int Action::contentContains(IQuestContent* content) {
	for (int i = 0; i < _concreteContent.size(); ++i) {
		if (_concreteContent[i].first == content) {
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
					if(_concreteContent[j].first != nullptr && !((BasicEnemy*)_concreteContent[j].first)->isDead()) {
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

						_concreteContent[j].first = randomEnemy;
						((PreSomebodyThere*)it->second)->characters.push_back(_concreteContent[j].first);
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
				if (_concreteContent[j].first->getType() == EnemyQC) {
					if (_concreteContent[j].first != nullptr && !((BasicEnemy*)_concreteContent[j].first)->isDead()) {
						((PreSomethingThere*)it->second)->character = _concreteContent[j].first;
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

						_concreteContent[j].first = randomEnemy;
						((PreSomethingThere*)it->second)->character = _concreteContent[j].first;
					}
				}
				if(_concreteContent[j].first->getType() == NPCQC){
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
	switch (_postcondition.first) {
	case YouHaveTheItem:
		break;
	case TheyHaveTheItem:
		for (int j = 0; j < _concreteContent.size(); ++j) {
			if (_concreteContent[j].first->getType() == NPCQC) {
				((PostTheyHaveItem*)_postcondition.second)->npc = _concreteContent[j].first;
			}
		}
		break;
	case YouInfo:
		for (int j = 0; j < _concreteContent.size(); ++j) {
			if (_concreteContent[j].first->getType() == NPCQC) {
				((PostYouInfo*)_postcondition.second)->npcWithInfo = _concreteContent[j].first;
			}
		}
		break;
	case TheyInfo:
		break;
	case YouThere:
		for (int j = 0; j < _concreteContent.size(); ++j) {
			if (_concreteContent[j].first->getType() == TownQC) {
				((PostYouThere*)_postcondition.second)->city = _concreteContent[j].first;
			}
		}
		break;
	case TheyDead:
		for (int j = 0; j < _concreteContent.size(); ++j) {
			if (_concreteContent[j].first->getType() == EnemyQC) {
				((PostTheyDead*)_postcondition.second)->enemy = _concreteContent[j].first;
			}
		}
		break;
	default:
		break;
	}
}

/// <summary>
/// Updates the ids of the required content list.
/// </summary>
/// <param name="pNewIds">The p new ids.</param>
void Action::updateIds(std::vector<int> pNewIds) {
	// need to member the original ids
	std::vector<int> oldIds;

	// save the old ids and update to the new
	for (int i = 0; i < _requiredContent.size(); ++i) {
		oldIds.push_back(_requiredContent[i].second);
		_requiredContent[i].second = pNewIds[i];
	}

	// info to replace templates from string
	QuestContent contentType;

	std::string newInstructionText = "";
	int id;
	std::string typeString;

	std::regex expr("([A-Z||a-z]+)([0-9]+)");	 // the template to match
	std::smatch match;										 // the matches to be found

	while (std::regex_search(_instruction, match, expr)) {
		// add everything before the found pattern to the return string
		newInstructionText += match.prefix().str();

		// find the type and the id in the string
		typeString = match[1].str();
		id = atoi(match[2].str().c_str());

		// find the enums for all the type strings found
		for (std::map<std::string, QuestContent>::iterator it = GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.begin();
			it != GameManager::getSingletonPtr()->getQuestManager()->stringToQuestContentType.end(); ++it) {
			if (typeString == it->first) {
				contentType = it->second;
			}
		}

		// the replace string contains at least the type string, but the id still needs to be updated
		std::string replaceString = typeString;

		// find the new id by counting the index of the found id and matching it with the index in the updated requiredcontent list
		int index = 0;
		for (int i = 0; i < oldIds.size(); ++i) {
			if(oldIds[i] == id && _requiredContent[i].first == contentType) {
				index = pNewIds[i];
				break;
			}
		}

		std::stringstream newId;
		newId << index;

		replaceString += newId.str();

		// add the replaced text
		newInstructionText += replaceString;

		// cut the processed part of the instruction
		_instruction = match.suffix().str();
	}

	// add the last part to the new instruction if no matches are found anymore
	if (_instruction != "") {
		newInstructionText += _instruction;
	}

	// set the instruction text to the newly build text with updated ids
	_instruction = newInstructionText;
}

void Action::sendMsg(IQuestContent* sender, std::string msg) {
	if(_postcondition.second != nullptr) {
		_postcondition.second->receiveMsg(sender, msg);
	}
}
