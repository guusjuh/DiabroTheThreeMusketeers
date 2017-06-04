#ifndef ENEMY_FOLLOW_DIRECT_STATE_H
#define ENEMY_FOLLOW_DIRECT_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyFollowDirectState : public State<BaseNpc>
{
public:
	EnemyFollowDirectState();
	~EnemyFollowDirectState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif