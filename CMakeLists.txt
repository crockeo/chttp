cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")

set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/out")

set(CHTTP_SOURCES
  src/headers.c
  src/parse.c
  src/print.c
)

add_library(chttp ${CHTTP_SOURCES})
