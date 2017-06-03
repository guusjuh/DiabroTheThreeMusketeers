#include "Catch.hpp"

#include "LevelGenerator.h"
#include "Zone.h"
#include "City.h"
#include "LevelAccess.h"

TEST_CASE("level generator : city spaces", "[level-generation][cities]") {
	
	SECTION("city sizes") {
		GIVEN("a new zone is created")
		{
			Zone zone = Zone(10, 10, 3, 3, 10, 100, 500);
			LevelAccess access = LevelAccess(zone);
				WHEN("maximum city size is 3")
			{
				THEN("no cities exist with a size bigger than 3")
				{
					REQUIRE(zone.cities.size() > 0);
					int maxX = 0;
					int maxZ = 0;
					for (int i = 0; i < zone.cities.size(); ++i)
					{
						maxX = (maxX < zone.cities[i].Width()) ? zone.cities[i].Width() : maxX;
						maxZ = (maxZ < zone.cities[i].Depth()) ? zone.cities[i].Depth() : maxZ;
					}
					REQUIRE(maxX <= 3);
					REQUIRE(maxZ <= 3);
				}
			}
			WHEN("Rooms and corridors are spawned")
			{
				THEN("all places in the dungeon are connected with eachother")
				{
					int max = access.getMaxValue();
					REQUIRE(max == 1);
				}
			}
		}
	}
}