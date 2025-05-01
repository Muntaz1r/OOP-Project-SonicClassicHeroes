cmake_minimum_required(VERSION 3.10)
project(SonicClassicHeroes)

set(CMAKE_CXX_STANDARD 17)

# Add 'classes' directory to the list of directories to search for header files
include_directories(${PROJECT_SOURCE_DIR}/classes)
include_directories(${PROJECT_SOURCE_DIR}/classes/game)
include_directories(${PROJECT_SOURCE_DIR}/classes/entities)
include_directories(${PROJECT_SOURCE_DIR}/classes/factories)




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
