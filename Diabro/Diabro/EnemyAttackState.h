#ifndef ENEMY_ATTACK_STATE_H
#define ENEMY_ATTACK_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyAttackState : public State<BaseNpc>
{
public:
	EnemyAttackState();
	~EnemyAttackState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif