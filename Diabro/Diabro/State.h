#ifndef STATE_H_
#define STATE_H_
#include <OgrePrerequisites.h>

template <typename T>
class State
{
public:
	State();
	~State();

	void Enter(T agent) {}
	void Execute(T agent) {}
	void Exit(T agent) {}
};

#endif