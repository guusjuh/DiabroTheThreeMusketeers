#include "EnemyRangedAttackState.h"

void EnemyRangedAttackState::Execute(BaseNpc* agent) {
	agent->lightAttack();
	agent->stateMachine.setState("Idle");
}