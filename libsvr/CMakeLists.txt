cmake_minimum_required (VERSION 2.80)

project (libsvr)

include(../CompileOptions.cmake)

aux_source_directory (src SRC_LIST)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
list(REMOVE_ITEM SRC_LIST "src/iocp_tcp.c")
endif()

add_library(${PROJECT_NAME} STATIC ${SRC_LIST})

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
