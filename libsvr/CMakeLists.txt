cmake_minimum_required (VERSION 2.80)

project (libsvr)
message(${PROJECT_NAME})

include(../CompileOptions.cmake)

#aux_source_directory (src SRC_FILES)

#FILE(GLOB INCLUDE_FILES "include/*.h" "include/*.hpp")
set(
    SRC_FILES
	"src/avl_tree.c"
	"src/base64.c"
	"src/client_mysql.c"
	"src/db_mysql.cpp"
	"src/epoll_tcp.c"
	"src/iocp_tcp.c"
	"src/json.c"
	"src/lib_svr_common_def.c"
	"src/lib_svr_common_def.h"
	"src/lock_free_queue.cpp"
	"src/logger.cpp"
	"src/loop_cache.c"
	"src/memory_pool.c"
	"src/memory_trace.cpp"
	"src/net_ssl.c"
	"src/rb_tree.c"
	"src/share_memory.c"
	"src/sha_1.c"
	"src/smemory.cpp"
	"src/timer.c"
	"src/tree_help.c"
	"src/utility.cpp"
	"src/websocket.c"
)

set(
    INC_FILES
    "include/avl_tree.h"
	"include/client_mysql.h"
	"include/db_cache.hpp"
	"include/db_fields.hpp"
	"include/db_mysql.hpp"
	"include/db_table.hpp"
	"include/format_args.hpp"
	"include/json.h"
	"include/lib_svr_def.h"
	"include/list_value_operation.hpp"
	"include/lock_free_queue.hpp"
	"include/logger.hpp"
	"include/loop_cache.h"
	"include/memory_pool.h"
	"include/memory_trace.hpp"
	"include/meyers_singleton.hpp"
	"include/net_data.hpp"
	"include/net_ssl.h"
	"include/net_tcp.h"
	"include/rb_tree.h"
	"include/share_memory.h"
	"include/smemory.hpp"
	"include/special_decay_type.hpp"
	"include/static_string.hpp"
	"include/task.hpp"
	"include/timer.h"
	"include/tree_help.h"
	"include/utility.hpp"
	"include/websocket.h"
)


#aux_source_directory (src libsvr_source_files)
#source_group("Source Files" FILES ${libsvr_source_files})

#file(GLOB_RECURSE libsvr_header_files  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
#source_group("Header Files" FILES ${libsvr_header_files})



source_group("inc" FILES ${INC_FILES})
source_group("src" FILES ${SRC_FILES})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
list(REMOVE_ITEM SRC_FILES "src/iocp_tcp.c")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcx16")
endif()

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${INC_FILES})
#add_library(${PROJECT_NAME} STATIC ${libsvr_source_files} ${libsvr_header_files})

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
endif()

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
