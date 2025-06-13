cmake_minimum_required(VERSION 3.10)
project(KingdomDefense)

set(CMAKE_CXX_STANDARD 17)

# Recherche tous les fichiers .cpp
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Crée l'exécutable
add_executable(KingdomDefense ${SRC_FILES})

# Inclut le dossier include/ (où sont tous les .h)
target_include_directories(KingdomDefense PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(KingdomDefense sfml-graphics sfml-window sfml-system)

# Copie les assets
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
