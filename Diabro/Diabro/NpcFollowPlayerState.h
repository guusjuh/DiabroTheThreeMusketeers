#ifndef NPC_FOLLOW_PLAYER_STATE_H
#define NPC_FOLLOW_PLAYER_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class NpcFollowPlayerState : public State<BaseNpc>
{
public:
	NpcFollowPlayerState();
	~NpcFollowPlayerState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
};
#endif