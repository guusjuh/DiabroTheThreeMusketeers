#include "State.h"
#include "Character.h"

/// <summary>
/// Initializes a new instance of the <see cref="State"/> class.
/// </summary>
template<typename T>
State<T>::State() {}

/// <summary>
/// Finalizes an instance of the <see cref="State"/> class.
/// </summary>
template<typename T>
State<T>::~State() {}

template class State<Character>;