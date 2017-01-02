find_path(CHTTP_INCLUDE_DIRS chttp_defines.h
                             chttp.h
          PATHS /usr/local/include)

find_library(CHTTP_LIBRARIES
             NAMES chttp
             PATHS /usr/local
             PATH_SUFFIXES lib/static
                           lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(chttp
                                  REQUIRED_VARS CHTTP_LIBRARIES
                                                CHTTP_INCLUDE_DIRS)
