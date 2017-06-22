#ifndef ENEMY_FOLLOW_DIRECT_STATE_H
#define ENEMY_FOLLOW_DIRECT_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// In this state, the enemy walks directly to the player. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyFollowDirectState : public State<BaseNpc>
{
public:
	EnemyFollowDirectState(){}
	~EnemyFollowDirectState(){}

	void Execute(BaseNpc* agent) override;

private:
	Ogre::Vector3 _playerPos;
};
#endif