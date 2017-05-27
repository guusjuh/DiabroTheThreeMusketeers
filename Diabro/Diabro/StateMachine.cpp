#include "StateMachine.h"
#include "Character.h"

/// <summary>
/// Initializes a new instance of the <see cref="StateMachine"/> class.
/// </summary>
/// <param name="owner">The owner of the fsm.</param>
/// <param name="startState">The start state.</param>
/// <param name="possibleStates">The possible states.</param>
template<typename T>
StateMachine<T>::StateMachine(T* owner, State<T> startState, std::map<std::string, State<T>> possibleStates)
{
	_owner = owner;
	_possibleStates = possibleStates;
	_currentState = nullptr;
	this->setState(startState);
}

/// <summary>
/// Initializes a new instance of the <see cref="StateMachine"/> class.
/// </summary>
template<typename T>
StateMachine<T>::StateMachine(){

}

/// <summary>
/// Finalizes an instance of the <see cref="StateMachine"/> class.
/// </summary>
template<typename T>
StateMachine<T>::~StateMachine(){

}

/// <summary>
/// Updates the current state of the fsm.
/// </summary>
template<typename T>
void StateMachine<T>::update()
{
	if (this->getCurrentState() != nullptr){
		this->getCurrentState()->Execute(_owner);
	}
}

/// <summary>
/// Gets the current state.
/// </summary>
/// <returns></returns>
template<typename T>
State<T>* StateMachine<T>::getCurrentState(){
	return _currentState;
}

/// <summary>
/// Ends the current state and strats the new state.
/// </summary>
/// <param name="newState">The new state.</param>
template<typename T>
void StateMachine<T>::setState(State<T> newState){
	_previous = _currentState;
	if (_currentState != nullptr){
		_currentState->Exit(_owner);
	}
	_currentState = &newState;
	_currentState->Enter(_owner);
}

template class StateMachine < Character > ;