#ifndef ENEMY_FOLLOW_STATE_H
#define ENEMY_FOLLOW_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyFollowState : public State<BaseNpc>
{
public:
	EnemyFollowState();
	~EnemyFollowState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif