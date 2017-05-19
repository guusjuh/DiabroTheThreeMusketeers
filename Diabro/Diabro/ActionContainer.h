#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include "GenericContainer.h"
#include "Action.h"

class ActionContainer : public GenericContainer<Action> {
public:
	ActionContainer();
	~ActionContainer();

	void readFromXML();
};

#endif