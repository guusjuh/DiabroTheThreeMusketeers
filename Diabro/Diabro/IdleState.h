#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class IdleState : public State<BaseNpc>
{
public:
	IdleState();
	~IdleState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif