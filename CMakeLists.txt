cmake_minimum_required(VERSION 3.10)

project(lab_06)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
  TLS_VERIFY false
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(lab_06
	main.cpp
	src/npc.cpp
	src/orc.cpp
	src/squirrel.cpp
	src/bear.cpp
)

target_include_directories(lab_06 PRIVATE include)

enable_testing()
add_executable(lab_06_tests
	tests/test.cpp
	src/npc.cpp
	src/orc.cpp
	src/squirrel.cpp
	src/bear.cpp
)
target_include_directories(lab_06_tests PRIVATE include)
target_link_libraries(lab_06_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(lab_06_tests)
