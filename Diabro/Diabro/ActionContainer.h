#ifndef ACTIONCONTAINER_H
#define ACTIONCONTAINER_H

#include "GenericContainer.h"
#include "Action.h"

class ActionContainer : public GenericContainer<Action> {
public:
	ActionContainer() {
		readFromXML();
	}

	~ActionContainer() {
		GenericContainer<Action>::~GenericContainer();
	}

	void readFromXML();
};

#endif