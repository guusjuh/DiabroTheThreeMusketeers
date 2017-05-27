#ifndef BASE_ENEMY_H_
#define BASE_ENEMY_H_

#include "BaseNpc.h"
#include "StateMachine.h"
#include "IdleState.h"

/// <summary>
/// The basic class for an enemy.
/// </summary>
/// <seealso cref="BaseNpc" />
class BasicEnemy : public BaseNpc
{
public:
	BasicEnemy(Ogre::SceneNode*, Ogre::SceneNode*, Ogre::Entity*, City*);
	~BasicEnemy() {}

	void update(Ogre::Real) override;
	void die() override;
	void updateBar(bool val) { _updateBar = val; }

	StateMachine<Character> stateMachine;

private:
	bool lightAttack() override;
	bool _updateBar;
};

#endif
