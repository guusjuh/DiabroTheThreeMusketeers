#ifndef ENEMY_ATTACK_STATE_H
#define ENEMY_ATTACK_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// In this state an enemy performs its attack. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyAttackState : public State<BaseNpc>
{
public:
	EnemyAttackState(){}
	~EnemyAttackState(){}

	void Execute(BaseNpc* agent) override;
};
#endif