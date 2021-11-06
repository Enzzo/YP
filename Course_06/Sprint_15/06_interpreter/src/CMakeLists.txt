cmake_minimum_required(VERSION 3.18)

project(06_interpreter CXX)
set(CMAKE_CXX_STANDARD 17)

set(MAIN_FILE main.cpp)
set(INCLUDE_DIR ../../../../include/interpreter)
set(LEXER_FILES 	"${INCLUDE_DIR}/lexer.h"
					"${INCLUDE_DIR}/lexer.cpp"
					"${INCLUDE_DIR}/lexer_test_open.cpp")
				
set(PARSER_FILES 	"${INCLUDE_DIR}/parse.h"
					"${INCLUDE_DIR}/parse.cpp"
					"${INCLUDE_DIR}/parse_test.cpp")
					
set(RUNTIME_FILES	"${INCLUDE_DIR}/runtime.h"
					"${INCLUDE_DIR}/runtime.cpp"
					"${INCLUDE_DIR}/runtime_test.cpp")
					
set(STATEMENT_FILES "${INCLUDE_DIR}/statement.h"
					"${INCLUDE_DIR}/statement.cpp"
					"${INCLUDE_DIR}/statement_test.cpp")
					
set(TEST_FILE "${INCLUDE_DIR}/test_runner_p.h")

source_group("lexer" FILES ${LEXER_FILES})
source_group("parser" FILES ${PARSER_FILES})
source_group("runtime" FILES ${RUNTIME_FILES})
source_group("statement" FILES ${STATEMENT_FILES})
source_group("test" FILES ${TEST_FILE})

add_executable(interpreter ${MAIN_FILE} ${LEXER_FILES} ${PARSER_FILES} ${RUNTIME_FILES} ${STATEMENT_FILES} ${TEST_FILE})

target_include_directories(interpreter PRIVATE ${INCLUDE_DIR})