#ifndef DEBUG_H_
#define DEBUG_H_
#include <cstdio>
#include <string>
#include "City.h"

class Debug
{
public:
	Debug(std::string msg, float digit);
	Debug(std::string msg, int digit);
	Debug(std::string msg);
	Debug(float digit);
	Debug(int digit);
	Debug(Coordinate coord);
	Debug(std::string msg, Coordinate coord);
	Debug();
	~Debug();

	void printArray(int* intArray, int width, int size) const;
	void printArray(bool* boolArray, int width, int size) const;

	FILE* fp;
};

#endif