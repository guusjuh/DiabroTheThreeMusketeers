#include "GenericContainer.h"
#include "Action.h"
#include "Strategy.h"

template<typename T>
GenericContainer<T>::GenericContainer() {}

template<typename T>
GenericContainer<T>::~GenericContainer() {
	for (int i = 0; i < _objects.size(); ++i) {
		if (_objects[i] != nullptr) {
			delete _objects[i];
		}
	}
}

template class GenericContainer< Action >; 
template class GenericContainer< Strategy >;
