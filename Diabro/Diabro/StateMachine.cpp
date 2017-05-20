#include "StateMachine.h"
#include "Character.h"

template<typename T>
StateMachine<T>::StateMachine(T* owner, State<T> startState, std::map<std::string, State<T>> possibleStates)
{
	_owner = owner;
	_possibleStates = possibleStates;
	_currentState = nullptr;
	this->setState(startState);
}

template<typename T>
StateMachine<T>::~StateMachine(){

}

template<typename T>
void StateMachine<T>::update()
{
	if (this->getCurrentState() != nullptr){
		this->getCurrentState()->Execute(_owner);
	}
}

template<typename T>
State<T>* StateMachine<T>::getCurrentState(){
	return _currentState;
}

template<typename T>
void StateMachine<T>::setState(State<T> newState){
	_previous = _currentState;
	if (_currentState != nullptr){
		_currentState->Exit(_owner);
	}
	_currentState = &newState;
	_currentState->Enter(_owner);
}

template class StateMachine < int > ;