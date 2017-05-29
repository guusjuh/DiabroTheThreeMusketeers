#ifndef GENERIC_FINITE_STATE_MACHINE_H
#define GENERIC_FINITE_STATE_MACHINE_H
#include "State.h"
#include <iostream>

/// <summary>
/// A generic Finite State Machine.
/// </summary>
template<typename T>
class StateMachine
{
public:
	StateMachine(T* owner, std::string startState, std::map<std::string, State<T>*> possibleStates);
	StateMachine();
	~StateMachine();

	std::map<std::string, State<T>*> _possibleStates;

	std::string getCurrentState();

	void setState(std::string newState);

	void update();

	void collide();

private:
	std::string _currentState;
	std::string _previous;
	T* _owner;
	
};

#endif
