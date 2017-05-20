#ifndef IDLE_STATE_H
#define IDLE_STATE_H
#include "State.h"
#include "BasicEnemy.h"

class IdleState : public State<BasicEnemy>
{
public:
	IdleState();
	~IdleState();

	void Enter(BasicEnemy agent);
	void Execute(BasicEnemy agent);
	void Exit(BasicEnemy agent);
};
#endif