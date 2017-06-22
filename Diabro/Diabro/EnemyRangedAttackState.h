#ifndef ENEMY_RANGED_ATTACK_STATE_H_
#define ENEMY_RANGED_ATTACK_STATE_H_

#include "State.h"
#include "BaseNpc.h"

class EnemyRangedAttackState : public State<BaseNpc> {
public:
	EnemyRangedAttackState(){}
	~EnemyRangedAttackState(){}

	void Enter(BaseNpc* agent) override{}
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override{}

private:
};

#endif