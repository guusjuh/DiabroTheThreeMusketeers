#ifndef STATE_H_
#define STATE_H_
#include <OgrePrerequisites.h>

/// <summary>
/// A generic state class.
/// </summary>
template <typename T>
class State
{
public:
	State();
	~State();

	void Enter(T* agent) {}
	void Execute(T* agent) {}
	void Exit(T* agent) {}
};

#endif