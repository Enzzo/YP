cmake_minimum_required(VERSION 3.18)

project(04_Lexer CXX)
set(CMAKE_CXX_STANDARD 17)

set(MAIN_FILE main.cpp)

set(INCLUDE_DIR ../../../../include/lexer)
set(INCLUDE_FILES 	"${INCLUDE_DIR}/lexer.h"
					"${INCLUDE_DIR}/lexer.cpp"
					"${INCLUDE_DIR}/lexer_test_open.cpp"
					"${INCLUDE_DIR}/test_runner_p.h")
source_group("include" FILES ${INCLUDE_FILES})
add_executable(lexer ${INCLUDE_FILES} ${MAIN_FILE})

target_include_directories(lexer PRIVATE ${INCLUDE_DIR})