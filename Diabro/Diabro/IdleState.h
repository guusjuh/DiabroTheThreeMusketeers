#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "State.h"

class IdleState : public State<int>
{
public:
	IdleState();
	~IdleState();

	void Enter(int* agent);
	void Execute(int* agent);
	void Exit(int* agent);
};
#endif