#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "State.h"
#include "Character.h"

class IdleState : public State<Character>
{
public:
	IdleState();
	~IdleState();

	void Enter(Character* agent);
	void Execute(Character* agent);
	void Exit(Character* agent);
};
#endif