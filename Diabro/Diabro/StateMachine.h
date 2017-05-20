#ifndef GENERIC_FINITE_STATE_MACHINE_H
#define GENERIC_FINITE_STATE_MACHINE_H
#include "State.h"
#include <iostream>

template<typename T>
class StateMachine
{
public:
	StateMachine(T* owner, State<T> startState, std::map<std::string, State<T>> possibleStates);
	StateMachine();
	~StateMachine();

	std::map<std::string, State<T>> _possibleStates;

	State<T>* getCurrentState();

	void setState(State<T> newState);

	void update();

private:
	State<T>* _currentState;
	State<T>* _previous;
	T* _owner;
	
};

#endif
