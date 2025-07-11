cmake_minimum_required(VERSION 3.18)

project(pt_1 CXX)
set(CMAKE_CXX_STANDARD 17)

set(MAIN_FILE main.cpp)
set(INCLUDE_DIR "include")
set(HEADER_FILES "${INCLUDE_DIR}/common.h" "${INCLUDE_DIR}/test_runner_p.h")
set(SOURCE_FILES "${INCLUDE_DIR}/structures.cpp")

add_executable(02 ${MAIN_FILE} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(02 PRIVATE Include)