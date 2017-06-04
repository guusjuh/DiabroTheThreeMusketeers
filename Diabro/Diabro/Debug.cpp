#include "Debug.h"
#include "math.h"
/// writes a message and a digit to the console
Debug::Debug(std::string msg, float digit)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf(msg.c_str());
	printf(" %f", digit);
	printf("\n");
	
#endif
}
/// writes a message and an integer to the console
Debug::Debug(std::string msg, int digit)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32

	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf(msg.c_str());
	printf(" %d", digit);
	printf("\n");

#endif
}
/// writes a message to the console
Debug::Debug(std::string msg) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf(msg.c_str());
	printf("\n");

#endif
}
/// writes a digit to the console
Debug::Debug(float digit) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf(" %f", digit);
	printf("\n");

#endif
}
/// writes an integer to the console
Debug::Debug(int digit) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf(" %d", digit);
	printf("\n");

#endif
}
/// writes a coordinate to the console
Debug::Debug(Coordinate coord) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("(%d, %d)", coord.x, coord.z);
	printf("\n");

#endif
}
/// writes a message and a coordinate to the console
Debug::Debug(std::string msg, Coordinate coord) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("%s (%d, %d)", msg.c_str(), coord.x, coord.z);
	printf("\n");

#endif
}
/// creates an empty debugger
Debug::Debug() {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	freopen_s(&fp, "CONOUT$", "w", stdout);
#endif
}

void Debug::printArray(int* intArray, int width, int size) const {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	for (int i = 0; i < size; i++) {
		if (width < size && width != 0 && i % width == 0) {
			printf("\n");
		}
		printf("|%d", intArray[i]);
		if (intArray[i] < 10) {
			printf("  |");
		} else {
			printf(" |");
		}
	}
	printf("\n");
#endif
}

void Debug::printArray(bool* boolArray, int width, int size) const {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	for (int i = 0; i < size; i++) {
		if (width < size && width != 0 && i % width == 0) {
			printf("\n");
		}
		printf("|%c", boolArray[i]?'.':'0');
		printf("|");
	}
	printf("\n");
#endif
}

Debug::~Debug()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	fclose(fp);
#endif
}
