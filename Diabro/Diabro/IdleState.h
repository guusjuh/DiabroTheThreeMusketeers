#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "State.h"
#include "Character.h"

/// <summary>
/// The concrete idle implementation for characters. 
/// </summary>
/// <seealso cref="State{Character}" />
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