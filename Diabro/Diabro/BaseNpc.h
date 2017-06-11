#ifndef BASE_NPC_H_
#define BASE_NPC_H_

#include "Character.h"
#include "City.h"
#include "StateMachine.h"

/// <summary>
/// The super class for all NPC's in the game
/// </summary>
/// <seealso cref="Character" />
/// <seealso cref="IQuestContent" />
class BaseNpc : public Character, public IQuestContent
{
public:
	BaseNpc(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City* );
	BaseNpc() {}
	~BaseNpc() {}

	void collide() override;
	void update(Ogre::Real) override;
	void rotatePivot(Ogre::Vector3);
	using Character::lightAttack;

	void calculateAStar(Ogre::Vector3 targetPos);
	void walkToNextPoint();
	void walkTo(Ogre::Vector3);
	void walkToNeighbour();

	StateMachine<BaseNpc> stateMachine;

	int getNextPosSize() { return nextPos.size(); }
	Coordinate getGoalPos() { return goalPos; }
	City* getCity() { return _myCity; }
	bool isPlayerDetected() { return _playerDetected; }
	City* getMyCity() { return _myCity; }
	using IQuestContent::getType;
	Ogre::Vector3 getQuestPosition() override { return getPosition(); }

	virtual bool talk();
	void recieveItem() override;

protected:
	std::vector<Coordinate> nextPos;
	bool _inCurrentQuest;

	Ogre::SceneNode* _myRotationNode;

	Coordinate goalPos;
	City* _myCity;
	bool _playerDetected;
	virtual void detectPlayer();

	bool _inDialog;						//!< True if the player is currently talking with this NPC.
	int _dialogCount;					//!< The amount of different parts the dialog consists of.
	std::vector<std::string> _dialog;

	void setDialog(std::string s) {
		_dialog.clear();

		std::string delimiter = "\\n";
		std::string token;
		size_t pos = 0;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			_dialog.push_back(token);
			s.erase(0, pos + delimiter.length());
		}

		if (s != "") _dialog.push_back(s);
	}
	void setRelevantForAction(bool val) override {
		// don't change if u don't have to
		if (_relevantForAction == val) return;

		IQuestContent::setRelevantForAction(val);

		// if you're relevant, set the indicator on with correct color
		if(_relevantForAction) {
			questIndicatorEntity->setMaterialName(_activeMaterial);
			indicatorNode->setVisible(true);
		} else {
			indicatorNode->setVisible(false);
		}
	}

	static const Ogre::String _questMaterial;
	static const Ogre::String _activeMaterial;
	Ogre::Entity* questIndicatorEntity;
	Ogre::SceneNode* indicatorNode;

private:
	float _timer;
	float _degreePerFrame;
	float _wantedRotationAngle;

	void wander(); 	
};
#endif