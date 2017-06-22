#ifndef ENEMY_CHARGE_STATE_H
#define ENEMY_CHARGE_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyChargeState : public State<BaseNpc>
{
public:
	EnemyChargeState();
	~EnemyChargeState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
private:
	float timer;
};
#endif