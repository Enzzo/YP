cmake_minimum_required(VERSION 3.18)

project(05_runtime CXX)
set(CMAKE_CXX_STANDARD 17)

set(MAIN_FILE main.cpp)

set(INCLUDE_DIR ../../../../include/runtime)
set(INCLUDE_FILES 	"${INCLUDE_DIR}/runtime.h"
					"${INCLUDE_DIR}/runtime.cpp"
					"${INCLUDE_DIR}/runtime_test.cpp"
					"${INCLUDE_DIR}/test_runner_p.h")
source_group("include" FILES ${INCLUDE_FILES})

add_executable(runtime ${MAIN_FILE} ${INCLUDE_FILES})

target_include_directories(runtime PRIVATE ${INCLUDE_DIR})