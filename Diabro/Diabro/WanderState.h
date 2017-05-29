#ifndef WANDER_STATE_H
#define WANDER_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete wander implementation for enemies. 
/// </summary>
/// <seealso cref="State{Character}" />
class WanderState : public State<BaseNpc>
{
public:
	WanderState();
	~WanderState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif