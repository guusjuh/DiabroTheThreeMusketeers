#ifndef NPC_KIDNAPPED_STATE_H
#define NPC_KIDNAPPED_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class NpcKidnappedState : public State<BaseNpc>
{
public:
	NpcKidnappedState();
	~NpcKidnappedState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif