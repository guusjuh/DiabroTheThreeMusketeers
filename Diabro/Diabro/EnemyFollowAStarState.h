#ifndef ENEMY_FOLLOW_A_STAR_STATE_H
#define ENEMY_FOLLOW_A_STAR_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// In this state the enemy follows the target with help of A* 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyFollowAStarState : public State<BaseNpc>
{
public:
	EnemyFollowAStarState() {}
	~EnemyFollowAStarState() {}

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;

private: 
	Ogre::Vector3 _targetPos;
	Ogre::Vector3 getPlayerAsTarget();
};
#endif