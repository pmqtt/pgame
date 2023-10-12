cmake_minimum_required(VERSION 3.10)
project(pgamelib)

find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Wpedantic -g)
add_compile_options(-fsanitize=address)
link_libraries(-fsanitize=address)

set(CURRENT_CMAKE_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ENGINE_PATH "${CURRENT_CMAKE_FILE_DIR}")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/core/pglobals.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/core/pglobals.h
)


add_library(pgame STATIC core/panimation.cpp 
                         core/peventloop.cpp 
                         core/pwindow.cpp
                         datastructs/phashmap.cpp
                         physic/pcollider.cpp
                         physic/pphysic.cpp
                         physic/pforce.cpp
            )

message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")





function(add_core_test TEST_NAME)
  add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
  #target_link_libraries(${TEST_NAME} SDL2::SDL2 pgame)
  target_link_libraries(${TEST_NAME} SDL2main SDL2 SDL2_ttf pgame)
  # Create directory in test/bin if it doesn't exist
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test/bin)

  # Move test executable to test/bin
  add_custom_command(TARGET ${TEST_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TEST_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/test/bin)

  # Remove test executable from build directory
  add_custom_command(TARGET ${TEST_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:${TEST_NAME}>)
endfunction()


function(add_game_test NAME)
    add_executable(${NAME} test/games/${NAME}/main.cpp)
    target_link_libraries(${NAME} SDL2main SDL2 SDL2_ttf pgame)
  
    add_custom_command(TARGET ${NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${NAME}> ${CMAKE_CURRENT_BINARY_DIR}/test/games/${NAME})
    # Remove test executable from build directory
    add_custom_command(TARGET ${NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:${NAME}>)
endfunction()

add_core_test(core_test_0001)
add_core_test(core_test_0002)
add_core_test(core_test_0003)
add_core_test(core_test_0004)
add_core_test(core_test_0005)
add_core_test(core_test_0006)
add_core_test(core_test_0007)
add_core_test(core_test_0008)


# GUI TESTS
add_core_test(gui_test_0001)

# GAME TESTS
add_game_test(tennis)

# DATASTRUCT TEST

add_core_test(datastruct_test_hashmap_0001)
