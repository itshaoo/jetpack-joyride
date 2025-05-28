cmake_minimum_required(VERSION 3.16)
project(Jetpack_Joyride)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    ptsd

    URL         https://github.com/ntut-open-source-club/practical-tools-for-simple-design/archive/refs/tags/v0.2.zip

    # Or alternatively if you want to use a specific git commit instead of a release:
    # GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
    # GIT_TAG         <git_tag>

    SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/PTSD
)

FetchContent_MakeAvailable(ptsd)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
list(TRANSFORM SRC_FILES PREPEND ${SRC_DIR}/)
list(TRANSFORM INCLUDE_FILES PREPEND ${INCLUDE_DIR}/)

# file(GLOB_RECURSE SRC_FILES src/*)
# file(GLOB_RECURSE HEADER_FILES include/*)
add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
        include/Background.hpp
        src/Background.cpp
        include/Logo.hpp
        src/Logo.cpp
        include/Animation.hpp
        src/Animation.cpp
        include/Object.hpp
        src/Object.cpp
        include/Player.hpp
        src/Player.cpp
        include/Coin.hpp
        src/Coin.cpp
        include/CoinManager.hpp
        src/CoinManager.cpp
        include/Zapper.hpp
        include/ZapperManager.hpp
        src/Zapper.cpp
        src/ZapperManager.cpp
        include/missile.hpp
        src/missile.cpp
        include/Equipment.hpp
        src/Equipment.cpp
        include/CollisionManager.hpp
        src/CollisionManager.cpp
        include/CoinCounter.hpp
        src/CoinCounter.cpp
        include/GravitySuit.hpp
        src/GravitySuit.cpp
        include/LilStomper.hpp
        src/LilStomper.cpp
        include/BestDistance.hpp
        include/DistanceCounter.hpp
        include/ILevel.hpp
        include/Level.hpp
        include/Level1.hpp
        include/Level2.hpp
        include/Level7.hpp
        include/LevelSelect.hpp
        include/MissionDescription.hpp
        include/PauseMenu.hpp
        src/BestDistance.cpp
        src/DistanceCounter.cpp
        src/Level1.cpp
        src/Level2.cpp
        src/Level7.cpp
        src/LevelSelect.cpp
        src/MissionDescription.cpp
        src/PauseMenu.cpp
        include/Level8.hpp
        src/Level8.cpp
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    message(FATAL_ERROR "relative RESOURCE_DIR is WIP, Please use `-DCMAKE_BUILD_TYPE=Debug` build for now.")
    # target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_INSTALL_PREFIX}/Resources")
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/PTSD/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
)
