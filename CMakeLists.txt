cmake_minimum_required(VERSION 3.10)
project(SonicClassicHeroes)

set(CMAKE_CXX_STANDARD 17)

# Add all relevant include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/classes
    ${PROJECT_SOURCE_DIR}/classes/entities
    ${PROJECT_SOURCE_DIR}/classes/entities/dynamicEntity
    ${PROJECT_SOURCE_DIR}/classes/entities/dynamicEntity/player
    ${PROJECT_SOURCE_DIR}/classes/entities/dynamicEntity/enemy
    ${PROJECT_SOURCE_DIR}/classes/entities/obstacle
    ${PROJECT_SOURCE_DIR}/classes/entities/collectibles
    ${PROJECT_SOURCE_DIR}/classes/factories
    ${PROJECT_SOURCE_DIR}/classes/game
)




# Find all source files recursively
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/classes/*.cpp
    ${PROJECT_SOURCE_DIR}/Src/*.cpp
)

add_executable(SonicClassicHeroes ${SOURCES})


# Find SFML (adjust version as needed)
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Link SFML
target_link_libraries(SonicClassicHeroes
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Copy the Data folder to the build directory after the project is built
add_custom_command(TARGET SonicClassicHeroes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Data $<TARGET_FILE_DIR:SonicClassicHeroes>/Data
)
