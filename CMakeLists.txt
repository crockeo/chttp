cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=c11")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/out")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/out")

##
# CHTTP Library
set(CHTTP_HEADERS
  src/lib/chttp_defines.h
  src/lib/chttp.h
)

set(CHTTP_SOURCES
  src/lib/headers.c
  src/lib/parse.c
  src/lib/print.c
  src/lib/io.c
)

add_library(chttp ${CHTTP_SOURCES})

##
# CHTTP Tests
set(CHTTP_TEST_SOURCES
  src/bin/test.c
)

add_executable(chttp_test ${CHTTP_TEST_SOURCES})
target_link_libraries(chttp_test chttp)

##
# CHTTP Server
set(CHTTP_SERVER_SOURCES
  src/bin/main.c
)

add_executable(chttp_server ${CHTTP_SERVER_SOURCES})
target_link_libraries(chttp_server chttp)

##
# Installation.
install(TARGETS chttp
                chttp_server
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(FILES       ${CHTTP_HEADERS}
        DESTINATION include)

install(FILES src/Findchttp.cmake
        DESTINATION "${CMAKE_ROOT}/Modules")
