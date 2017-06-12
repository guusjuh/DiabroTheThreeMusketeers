#ifndef NPC_WANDER_STATE_H
#define NPC_WANDER_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class NpcWanderState: public State<BaseNpc>
{
public:
	NpcWanderState();
	~NpcWanderState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif