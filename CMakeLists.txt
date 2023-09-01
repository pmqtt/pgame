cmake_minimum_required(VERSION 3.10)
project(pgamelib)

find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-fsanitize=address)
link_libraries(-fsanitize=address)

add_executable(pgamelib main.cpp  panimation.cpp  peventloop.cpp pwindow.cpp)
target_link_libraries(pgamelib SDL2::SDL2)
