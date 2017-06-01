#ifndef ENEMY_MOVE_AROUND_CENTER_STATE_H
#define ENEMY_MOVE_AROUND_CENTER_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyMoveAroundCenterState: public State<BaseNpc>
{
public:
	EnemyMoveAroundCenterState();
	~EnemyMoveAroundCenterState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
	void Collide(BaseNpc* agent) override;
	int cornerId;
	void setNextPointList(BaseNpc* agent);
};
#endif