#include "GenericContainer.h"
#include "Action.h"
#include "Strategy.h"

template<typename T>
GenericContainer<T>::GenericContainer() {}

template<typename T>
GenericContainer<T>::~GenericContainer() {}

template class GenericContainer< Action >; 
template class GenericContainer< Strategy >;
