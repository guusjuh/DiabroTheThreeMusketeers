#ifndef ENEMY_WALK_TO_POINT_NEAR_PLAYER_STATE_H
#define ENEMY_WALK_TO_POINT_NEAR_PLAYER_STATE_H

#include "State.h"
#include "BaseNpc.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
class EnemyWalkToPointNearPlayerState : public State<BaseNpc>
{
public:
	EnemyWalkToPointNearPlayerState();
	~EnemyWalkToPointNearPlayerState();

	void Enter(BaseNpc* agent) override;
	void Execute(BaseNpc* agent) override;
	void Exit(BaseNpc* agent) override;
private:
	void calculateRelativePoint();

	Ogre::Vector3 relative;
};
#endif