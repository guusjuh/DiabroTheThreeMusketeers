#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include "GenericContainer.h"
#include "Action.h"

/// <summary>
/// The container for the actions that can be used in a quest. 
/// </summary>
/// <seealso cref="GenericContainer{Action}" />
class ActionContainer : public GenericContainer<Action> {
public:
	ActionContainer();
	~ActionContainer();

	void readFromXML();
};

#endif