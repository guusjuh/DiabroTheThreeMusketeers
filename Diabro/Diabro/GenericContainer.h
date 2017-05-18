#ifndef GENERIC_CONTAINER_H
#define GENERIC_CONTAINER_H

#include <vector>

template<typename T>
class GenericContainer {
public:
	GenericContainer();
	~GenericContainer() {
		for (int i = 0; i < _objects.size(); ++i) {
			if (_objects[i] != nullptr) {
				delete _objects[i];
			}
		}
	}

	int GetAmount() { return _amount; }
	std::vector<T*> GetObjects() { return _objects; }

protected:
	int _amount;
	std::vector<T*> _objects;
};
#endif 
