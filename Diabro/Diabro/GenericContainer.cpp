#include "GenericContainer.h"
#include "Action.h"
#include "Strategy.h"

/// <summary>
/// Initializes a new instance of the <see cref="GenericContainer"/> class.
/// </summary>
template<typename T>
GenericContainer<T>::GenericContainer() {}

/// <summary>
/// Finalizes an instance of the <see cref="GenericContainer"/> class.
/// </summary>
template<typename T>
GenericContainer<T>::~GenericContainer() {}

template class GenericContainer< Action >; 
template class GenericContainer< Strategy >;
