cmake_minimum_required(VERSION 3.10)
project(KingdomDefense)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find SFML version 3 with required modules
find_package(SFML 3 REQUIRED COMPONENTS graphics audio window system)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML 3 not found! Please install SFML 3 and try again.")
endif()

# Collect all .cpp files in the root directory (for tower.cpp, projectile.cpp, etc.)
file(GLOB ROOT_CPP "*.cpp")

# Collect all .cpp files from the src/ directory (for enemy, map, wave, etc.)
file(GLOB_RECURSE SRC_CPP "src/*.cpp")

# Define the main executable and include all collected source files
add_executable(KingdomDefense ${ROOT_CPP} ${SRC_CPP})

# Add include/ directory to the compiler's header search path
target_include_directories(KingdomDefense PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Link against SFML libraries
target_link_libraries(KingdomDefense
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
)

# Automatically copy the assets folder to the build directory,
# so that images, sounds, and maps are found at runtime
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
