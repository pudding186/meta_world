cmake_minimum_required (VERSION 2.80)

set (CMAKE_SYSTEM_VERSION   "10.0.17134.0")
set (RELEASE_TYPE "RelWithDebInfo")

project(meta_world)

SET(VERSION_TAG "0")
SET(VERSION_SIGN "0")

IF(EXISTS "${CMAKE_SOURCE_DIR}/.git/")
    FIND_PACKAGE(Git QUIET)
    
    IF(GIT_FOUND)
        execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
        OUTPUT_VARIABLE VERSION_SIGN
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        
        execute_process(
        COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
        OUTPUT_VARIABLE VERSION_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    ENDIF(GIT_FOUND)
ENDIF(EXISTS "${CMAKE_SOURCE_DIR}/.git/")

MESSAGE(STATUS "VERSION_TAG=${VERSION_TAG}")
MESSAGE(STATUS "VERSION_SIGN=${VERSION_SIGN}")
MESSAGE(STATUS "PROJECT_NAME=${PROJECT_NAME}")

configure_file(
"${CMAKE_CURRENT_SOURCE_DIR}/version.h.cfg"
"${CMAKE_CURRENT_SOURCE_DIR}/common/version.h"
)

add_subdirectory(libsvr)
add_subdirectory(lua-5.4.4)
add_subdirectory(agentserver)
add_subdirectory(dataserver)
#add_subdirectory(worldserver)
#add_subdirectory(databaseserver)
#add_subdirectory(gameserver)
#add_subdirectory(chatserver)
#add_subdirectory(databaseserverex)


