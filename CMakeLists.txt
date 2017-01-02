cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c11")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/out")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/out")

set(CHTTP_HEADERS
  src/chttp_defines.h
  src/chttp.h
)

set(CHTTP_SOURCES
  src/headers.c
  src/parse.c
  src/print.c
  src/io.c
)

add_library(chttp ${CHTTP_SOURCES})

set(CHTTP_TEST_SOURCES
  src/test.c
)

add_executable(chttp-test ${CHTTP_TEST_SOURCES})
target_link_libraries(chttp-test chttp)

# Installation.
install(TARGETS chttp
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(FILES       ${CHTTP_HEADERS}
        DESTINATION include)

install(FILES src/Findchttp.cmake
        DESTINATION "${CMAKE_ROOT}/Modules")
