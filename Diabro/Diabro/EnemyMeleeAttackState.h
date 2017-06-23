#ifndef ENEMY_ATTACK_STATE_H
#define ENEMY_ATTACK_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// In this state an enemy performs its attack. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyMeleeAttackState : public State<BaseNpc>
{
public:
	EnemyMeleeAttackState(){}
	~EnemyMeleeAttackState(){}

	void Execute(BaseNpc* agent) override;
};
#endif