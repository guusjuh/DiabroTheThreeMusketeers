#ifndef NPC_IDLE_STATE_H
#define NPC_IDLE_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class NpcIdleState : public State<BaseNpc>
{
public:
	NpcIdleState();
	~NpcIdleState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif