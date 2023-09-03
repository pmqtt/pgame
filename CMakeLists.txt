cmake_minimum_required(VERSION 3.10)
project(pgamelib)

find_package(SDL2 REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-fsanitize=address)
link_libraries(-fsanitize=address)
add_library(pgame STATIC panimation.cpp peventloop.cpp pwindow.cpp)


function(add_core_test TEST_NAME)
  add_executable(${TEST_NAME} test/${TEST_NAME}.cpp)
  target_link_libraries(${TEST_NAME} SDL2::SDL2 pgame)

  # Create directory in test/bin if it doesn't exist
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test/bin)

  # Move test executable to test/bin
  add_custom_command(TARGET ${TEST_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TEST_NAME}> ${CMAKE_CURRENT_BINARY_DIR}/test/bin)

  # Remove test executable from build directory
  add_custom_command(TARGET ${TEST_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE:${TEST_NAME}>)
endfunction()

add_core_test(core_test_0001)
add_core_test(core_test_0002)
