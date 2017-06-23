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
	EnemyMoveAroundCenterState(std::string next) {
		nextState = next;
	}
	~EnemyMoveAroundCenterState() {}

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;

private:
	int _cornerId;
	void setNextRoute(BaseNpc* agent);

	std::string nextState;
};
#endif