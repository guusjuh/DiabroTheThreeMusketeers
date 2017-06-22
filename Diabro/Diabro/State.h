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

	virtual void Enter(T* agent) {}
	virtual void Execute(T* agent) {}
	virtual void Exit(T* agent) {}
};

#endif