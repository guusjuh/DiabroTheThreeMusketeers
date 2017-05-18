#include "Catch.hpp"
#include "Zone.h"


TEST_CASE("CollisionGridTest")
{
	SECTION("bool adjustHealth(float) override")
	{
		SECTION("checks if function doesn’t throw anything")
		{
			REQUIRE(1 == 1);
			REQUIRE_FALSE(1 == 0);

		}

	}
}


/*
bool foo = false;
for (int i = 0; i < _width; ++i){
	for (int iz = 0; iz < _depth; ++iz){
		if (collisionGrid[i + iz * _width] && getTile(i, iz)){
		}
		else{
			bool foo = true;
		}
	}
}

if (foo){
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("aaaaaaaand you fail\n");
	fclose(fp);
#endif
}
else{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	printf("aaaaaaaand you fail\n");
	fclose(fp);
#endif
}*/