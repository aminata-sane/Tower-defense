cmake_minimum_required(VERSION 3.10)
project(KingdomDefense)

set(CMAKE_CXX_STANDARD 17)

# Recherche tous les fichiers .cpp
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Crée l'exécutable
add_executable(KingdomDefense ${SRC_FILES})

# Inclut le dossier include/
target_include_directories(KingdomDefense PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# Ajoute le dossier de config SFML explicitement
set(SFML_DIR "/opt/homebrew/lib/cmake/SFML")

find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
target_link_libraries(KingdomDefense PRIVATE sfml-graphics sfml-window sfml-system)

# Copie les assets
file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
