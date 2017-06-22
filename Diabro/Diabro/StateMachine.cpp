#include "StateMachine.h"
#include "BaseNpc.h"
#include "Debug.h"

/// <summary>
/// Initializes a new instance of the <see cref="StateMachine"/> class.
/// </summary>
/// <param name="owner">The owner of the fsm.</param>
/// <param name="startState">The start state.</param>
/// <param name="possibleStates">The possible states.</param>
template<typename T>
StateMachine<T>::StateMachine(T* owner, std::string startState, std::map<std::string, State<T>*> possibleStates)
{
	_owner = owner;
	_possibleStates = possibleStates;
	_currentState = "";
	setState(startState);
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
	if (this->getCurrentState() != ""){
		State<T>* current = _possibleStates[this->getCurrentState()];
		current->Execute(_owner);
	}
}

/// <summary>
/// Updates collision for the current state of the fsm.
/// </summary>
/*template<typename T>
void StateMachine<T>::collide()
{
	if (this->getCurrentState() != ""){
		State<T>* current = _possibleStates[this->getCurrentState()];
		current->Collide(_owner);
	}
}*/

/// <summary>
/// Gets the current state.
/// </summary>
/// <returns></returns>
template<typename T>
std::string StateMachine<T>::getCurrentState(){
	return _currentState;
}

/// <summary>
/// Ends the current state and strats the new state.
/// </summary>
/// <param name="newState">The new state.</param>
template<typename T>
void StateMachine<T>::setState(std::string newState){
	_previous = _currentState;
	if (_currentState != ""){
		_possibleStates[_currentState]->Exit(_owner);
	}
	_currentState = newState.c_str();
	_possibleStates[_currentState]->Enter(_owner);
	//Debug(_currentState.c_str());
}

template class StateMachine <BaseNpc> ;