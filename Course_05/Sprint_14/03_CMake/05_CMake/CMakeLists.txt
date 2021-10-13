cmake_minimum_required (VERSION 3.21)

project(05_cmake CXX)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR "D:/GitHub/yp/include/CMake")
set(MAIN_DIR "D:/GitHub/yp/Course_05/Sprint_14/03_CMake/05_CMake")

set(HEADERS_SRC "${INCLUDE_DIR}/img_lib.h" "${INCLUDE_DIR}/ppm_image.h")
set(FILES_SRC "${INCLUDE_DIR}/img_lib.cpp" "${INCLUDE_DIR}/ppm_image.cpp")

set(HMIRROR_SRC "${INCLUDE_DIR}/hmirr.cpp")
set(VMIRROR_SRC "${INCLUDE_DIR}/vmirr.cpp")
set(SOBEL_SRC   "${INCLUDE_DIR}/sobel.cpp")

source_group("headers" FILES ${HEADERS_SRC})
source_group("src" FILES ${FILES_SRC})

add_executable(hmirror "${HEADERS_SRC}" "${FILES_SRC}" "${HMIRROR_SRC}")
add_executable(vmirror "${HEADERS_SRC}" "${FILES_SRC}" "${VMIRROR_SRC}")
add_executable(sobel   "${HEADERS_SRC}" "${FILES_SRC}" "${SOBEL_SRC}")