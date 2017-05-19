#ifndef GENERIC_CONTAINER_H
#define GENERIC_CONTAINER_H

#include <vector>

template<typename T>
class GenericContainer {
public:
	GenericContainer();
	~GenericContainer();

	int GetAmount() { return _amount; }
	std::vector<T*> GetObjects() { return _objects; }

protected:
	int _amount;
	std::vector<T*> _objects;
};
#endif 
