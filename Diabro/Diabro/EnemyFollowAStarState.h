#ifndef ENEMY_FOLLOW_A_STAR_STATE_H
#define ENEMY_FOLLOW_A_STAR_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyFollowAStarState : public State<BaseNpc>
{
public:
	EnemyFollowAStarState();
	~EnemyFollowAStarState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
	void Collide(BaseNpc* agent) override;
};
#endif