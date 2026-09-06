cmake_minimum_required(VERSION 3.16)

project(coding-dojo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Emit compile_commands.json so VS Code IntelliSense picks up flags.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Boost.Test is required by the task test cases (see Task00N/README.md).
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

enable_testing()

# ---------------------------------------------------------------------------
# Each task lives in its own folder and maps to one test executable.
# Add a target + test per task, e.g.:
#
#   add_executable(task001 Task001/task001.cpp Task001/task001_test.cpp)
#   target_link_libraries(task001 PRIVATE Boost::unit_test_framework)
#   add_test(NAME task001 COMMAND task001)
# ---------------------------------------------------------------------------

add_executable(task001 Task001/task001.cpp Task001/task001_test.cpp)
target_link_libraries(task001 PRIVATE Boost::unit_test_framework)
target_include_directories(task001 PRIVATE Task001)
add_test(NAME task001 COMMAND task001)

add_executable(task002 Task002/task002.cpp Task002/task002_test.cpp)
target_link_libraries(task002 PRIVATE Boost::unit_test_framework)
target_include_directories(task002 PRIVATE Task002)
add_test(NAME task002 COMMAND task002)

add_executable(task003 Task003/task003.cpp Task003/task003_test.cpp)
target_link_libraries(task003 PRIVATE Boost::unit_test_framework)
target_include_directories(task003 PRIVATE Task003)
add_test(NAME task003 COMMAND task003)

add_executable(task004 Task004/task004.cpp Task004/task004_test.cpp)
target_link_libraries(task004 PRIVATE Boost::unit_test_framework)
target_include_directories(task004 PRIVATE Task004)
add_test(NAME task004 COMMAND task004)

add_executable(task005 Task005/lru_cache.cpp Task005/lru_cache_test.cpp)
target_link_libraries(task005 PRIVATE Boost::unit_test_framework)
target_include_directories(task005 PRIVATE Task005)
add_test(NAME task005 COMMAND task005)

