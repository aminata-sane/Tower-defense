# cmake_minimum_required(VERSION 3.10)
# project(KingdomDefense)
# set(CMAKE_CXX_STANDARD 17)

# find_package(SFML 3 REQUIRED graphics audio window system)

# include_directories(include)
# file(GLOB SOURCES "src/*.cpp")
# add_executable(KingdomDefense ${SOURCES})
# target_link_libraries(KingdomDefense sfml-graphics sfml-audio sfml-window sfml-system)
cmake_minimum_required(VERSION 3.10)
project(KingdomDefense)
set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED graphics audio window system)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML 3 not found! Please install SFML 3 and try again.")
endif()

file(GLOB SOURCES "*.cpp")

add_executable(KingdomDefense ${SOURCES})

target_link_libraries(KingdomDefense
    sfml-graphics
    sfml-audio
    sfml-window
    sfml-system
)
