cmake_minimum_required(VERSION 3.27)
project(sw_battle_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- IO layer (provided test client) ---
add_library(sw_io STATIC
    src/IO/System/CommandParser.cpp
)
target_include_directories(sw_io PUBLIC src)

# --- Battle ---
add_library(sw_battle STATIC
    src/Battle/Battle.cpp
    src/Battle/Map.cpp
    src/Battle/Units/UnitRegistry.cpp
    src/Battle/BattleState.cpp
    src/Battle/Queries/BattleQueries.cpp
    src/Battle/Queries/CellOccupancy.cpp
    src/Battle/Queries/CombatQueries.cpp
    src/Battle/Queries/MarchMovement.cpp
    src/Battle/Units/UnitDefinitions.cpp
    src/Battle/Units/UnitFactory.cpp
    src/Battle/Commands/Internal/Mechanics.cpp
    src/Battle/Commands/MeleeAttackCommand.cpp
    src/Battle/Commands/RangedAttackCommand.cpp
    src/Battle/Commands/MoveMarchCommand.cpp
    src/Battle/Units/Behaviors/SwordsmanBehavior.cpp
    src/Battle/Units/Behaviors/HunterBehavior.cpp
    src/Battle/Units/Behaviors/UnitBehaviorRegistry.cpp
)
target_include_directories(sw_battle PUBLIC src)
target_link_libraries(sw_battle PUBLIC sw_io)

# --- Application ---
add_executable(sw_battle_test src/main.cpp)
target_link_libraries(sw_battle_test PRIVATE sw_battle sw_io)
