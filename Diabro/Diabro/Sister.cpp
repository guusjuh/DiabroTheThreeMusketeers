#include "Sister.h"
#include "GameManager.h"

/// <summary>
/// Creates a new instance of the <see cref="Sister"/> class.
/// </summary>
/// <param name="pMyNode">My node.</param>
/// <param name="pMyEntity">My entity.</param>
Sister::Sister(Ogre::SceneNode* pMyNode, Ogre::SceneNode* pMyRotationNode, Ogre::Entity* pMyEntity, City* pMyCity)
	: BaseNpc(pMyNode, pMyRotationNode, pMyEntity, pMyCity), _hometown(pMyCity), _initialized(false)
{
	// set the states of the FSM
	possibleStates["Wander"] = new NpcWanderState();
	possibleStates["FollowPlayer"] = new NpcFollowPlayerState();
	possibleStates["Idle"] = new NpcIdleState();
	possibleStates["Kidnapped"] = new NpcKidnappedState();

	GameManager::getSingletonPtr()->getLevelManager()->subscribeSister(this);

	rotatePivot(Ogre::Vector3(0, 90, 0));

	pMyNode->setScale(0.25f, 0.25f, 0.25f);
	pMyNode->setPosition(pMyNode->getPosition().x, 18.0f, pMyNode->getPosition().z);
	indicatorNode->setScale(0.3f, 0.3f, 0.3f);
	indicatorNode->setPosition(0.0f, 150.0f, 0.0f);

	_movespeed = 200;
	_rotationspeed = 180.0f;
	_noticeDistance = 400;
	_radius = 25.0f;

	goalPos = Coordinate(pMyNode->getPosition().x, pMyNode->getPosition().z);
}

/// <summary>
/// Finalizes an instance of the <see cref=""/> class.
/// </summary>
Sister::~Sister() {
}

/// <summary>
/// Updates the frame based on the specified deltatime.
/// </summary>
/// <param name="pDeltatime">The time since last frame.</param>
void Sister::update(Ogre::Real pDeltatime)
{
	// if we don't have a quest yet and we arn't initialized
	// this is contained in the update due to order problems when done in constructor
	if (!_initialized) {
		stateMachine = StateMachine<BaseNpc>(this, "Idle", possibleStates);
		_initialized = true;
	}

	BaseNpc::update(pDeltatime);
}

void Sister::collide() {
	walkToNeighbour();
}