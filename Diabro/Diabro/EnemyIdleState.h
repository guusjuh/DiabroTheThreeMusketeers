#ifndef ENEMY_IDLE_STATE_H_
#define ENEMY_IDLE_STATE_H_

#include "State.h"
#include "BaseNpc.h"

class EnemyIdleState : public State<BaseNpc> {
public:
	EnemyIdleState() {}
	~EnemyIdleState() {}

	void Enter(BaseNpc* agent) override {}
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override {}

private:
};

#endif