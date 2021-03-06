#/*
#-----------------------------------------------------------------------------
#Filename:    CMakeLists.txt
#-----------------------------------------------------------------------------
#
#This source file is part of the
#   ___                 __    __ _ _    _ 
#  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
# //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
#/ \_// (_| | | |  __/  \  /\  /| |   <| |
#\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
#      |___/                              
#      Tutorial Framework
#      http://www.ogre3d.org/tikiwiki/
#-----------------------------------------------------------------------------
#*/
cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)
	
set(APP "Diabro")
 
set(HDRS
	./Diabro/Diabro/BaseApplication.h
	./Diabro/Diabro/GameManager.h
	./Diabro/Diabro/Player.h
	./Diabro/Diabro/Npc.h
	./Diabro/Diabro/LevelManager.h
	./Diabro/Diabro/BasicEnemy.h
	./Diabro/Diabro/Character.h
	./Diabro/Diabro/BaseNpc.h
	./Diabro/Diabro/UIManager.h
	./Diabro/Diabro/Spawner.h
	./Diabro/Diabro/LevelGenerator.h
	./Diabro/Diabro/Zone.h
	./Diabro/Diabro/City.h
	./Diabro/Diabro/StateMachine.h
	./Diabro/Diabro/State.h
	./Diabro/Diabro/IdleState.h
	./Diabro/Diabro/WanderState.h
	./Diabro/Diabro/EnemyFollowAStarState.h
	./Diabro/Diabro/EnemyFollowDirectState.h
	./Diabro/Diabro/EnemyMoveAroundCenterState.h
	./Diabro/Diabro/EnemyAttackState.h
	./Diabro/Diabro/EnemyWalkToPointNearPlayerState.h
	./Diabro/Diabro/Needs.h
	./Diabro/Diabro/IQuestContent.h	
	./Diabro/Diabro/QuestContentManager.h 
	./Diabro/Diabro/QuestItem.h 
	./Diabro/Diabro/QuestItemContainer.h 
	./Diabro/Diabro/QuestItemGenerator.h
	./Diabro/Diabro/Action.h
	./Diabro/Diabro/ActionContainer.h
	./Diabro/Diabro/PostCondition.h
	./Diabro/Diabro/PostTheyHaveItem.h
	./Diabro/Diabro/PostYouHaveItem.h
	./Diabro/Diabro/PostYouThere.h
	./Diabro/Diabro/PostTheyDead.h
	./Diabro/Diabro/PostTheyInfo.h
	./Diabro/Diabro/PostYouInfo.h
	./Diabro/Diabro/PreCondition.h
	./Diabro/Diabro/PreKnowWhereToGo.h
	./Diabro/Diabro/PreSomebodyThere.h
	./Diabro/Diabro/PreSomethingThere.h
	./Diabro/Diabro/PreYouItemOfInterest.h
	./Diabro/Diabro/GenericContainer.h
	./Diabro/Diabro/Strategy.h
	./Diabro/Diabro/StrategyContainer.h
	./Diabro/Diabro/QuestManager.h
	./Diabro/Diabro/tinyxml2.h
	./Diabro/Diabro/BuildingType.h
	./Diabro/Diabro/CityManager.h
	./Diabro/Diabro/Quest.h
	./Diabro/Diabro/Node.h
	./Diabro/Diabro/QuestGenerator.h
	./Diabro/Diabro/SoundManager.h
	./Diabro/Diabro/IDecorator.h
	./Diabro/Diabro/IPlayerEquipment.h
	./Diabro/Diabro/PlayerEquipment.h
	./Diabro/Diabro/PlayerUpgrade.h
	./Diabro/Diabro/PlayerHealthUpgrade.h
	./Diabro/Diabro/PlayerDamageUpgrade.h
	./Diabro/Diabro/Debug.h
	./Diabro/Diabro/EnemyEquipment.h
	./Diabro/Diabro/EnemyUpgrade.h
	./Diabro/Diabro/IEnemyEquipment.h
	./Diabro/Diabro/Coordinate.h
)
 
set(SRCS
	./Diabro/Diabro/BaseApplication.cpp
	./Diabro/Diabro/GameManager.cpp
	./Diabro/Diabro/Player.cpp
	./Diabro/Diabro/Npc.cpp
	./Diabro/Diabro/LevelManager.cpp
	./Diabro/Diabro/BasicEnemy.cpp
	./Diabro/Diabro/Character.cpp
	./Diabro/Diabro/BaseNpc.cpp
	./Diabro/Diabro/Npc.cpp
	./Diabro/Diabro/UIManager.cpp
	./Diabro/Diabro/Spawner.cpp
	./Diabro/Diabro/QuestContentManager.cpp
	./Diabro/Diabro/GenericContainer.cpp
	./Diabro/Diabro/QuestItemContainer.cpp
	./Diabro/Diabro/QuestItemGenerator.cpp
	./Diabro/Diabro/Action.cpp
	./Diabro/Diabro/ActionContainer.cpp
	./Diabro/Diabro/PostCondition.cpp
	./Diabro/Diabro/PostTheyHaveItem.cpp
	./Diabro/Diabro/PostYouHaveItem.cpp
	./Diabro/Diabro/PostYouThere.cpp
	./Diabro/Diabro/PostTheyDead.cpp
	./Diabro/Diabro/PostTheyInfo.cpp
	./Diabro/Diabro/PostYouInfo.cpp
	./Diabro/Diabro/PreKnowWhereToGo.cpp
	./Diabro/Diabro/PreSomebodyThere.cpp
	./Diabro/Diabro/PreSomethingThere.cpp
	./Diabro/Diabro/PreYouItemOfInterest.cpp
	./Diabro/Diabro/StrategyContainer.cpp
	./Diabro/Diabro/QuestManager.cpp
	./Diabro/Diabro/LevelGenerator.cpp
	./Diabro/Diabro/Zone.cpp
	./Diabro/Diabro/City.cpp
	./Diabro/Diabro/StateMachine.cpp
	./Diabro/Diabro/State.cpp
	./Diabro/Diabro/IdleState.cpp
	./Diabro/Diabro/WanderState.cpp
	./Diabro/Diabro/EnemyFollowAStarState.cpp
	./Diabro/Diabro/EnemyFollowDirectState.cpp
	./Diabro/Diabro/EnemyMoveAroundCenterState.cpp
	./Diabro/Diabro/EnemyAttackState.cpp
	./Diabro/Diabro/EnemyWalkToPointNearPlayerState.cpp
	./Diabro/Diabro/tinyxml2.cpp
	./Diabro/Diabro/BuildingType.cpp
	./Diabro/Diabro/CityManager.cpp
	./Diabro/Diabro/Quest.cpp
	./Diabro/Diabro/Node.cpp
	./Diabro/Diabro/QuestGenerator.cpp
	./Diabro/Diabro/PlayerEquipment.cpp
	./Diabro/Diabro/SoundManager.cpp
	./Diabro/Diabro/PlayerUpgrade.cpp
	./Diabro/Diabro/PlayerHealthUpgrade.cpp
	./Diabro/Diabro/PlayerDamageUpgrade.cpp
	./Diabro/Diabro/Debug.cpp
	./Diabro/Diabro/EnemyEquipment.cpp
	./Diabro/Diabro/EnemyUpgrade.cpp
)
 
include("tools/cmake/common.cmake") 
