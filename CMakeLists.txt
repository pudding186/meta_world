cmake_minimum_required (VERSION 2.80)
set(
    NAME
    "meta_world"
)
SET (CMAKE_SYSTEM_VERSION   "10.0.17134.0")
SET (RELEASE_TYPE "RelWithDebInfo")

project(${NAME})

STRING( REGEX REPLACE ".*/(.*)" "\\1" VERSION_TIME ${CMAKE_SOURCE_DIR} )

SET(ROOT_DIR "${PROJECT_SOURCE_DIR}/../")

SET(VERSION_REVISION "1")
set(MS_VERSION "MH2_${VERSION_TIME}")

IF(EXISTS "${ROOT_DIR}/.svn/")
  FIND_PACKAGE(Subversion)

  IF(SUBVERSION_FOUND)
    Subversion_WC_INFO(${ROOT_DIR} Project)
    SET(VERSION_REVISION ${Project_WC_REVISION})
  ENDIF(SUBVERSION_FOUND)
ENDIF(EXISTS "${ROOT_DIR}/.svn/")

MESSAGE(STATUS "VERSION=${VERSION_REVISION}")

SET(ROOT_DIR ${PROJECT_SOURCE_DIR})
IF(EXISTS "${ROOT_DIR}/.svn/")
  FIND_PACKAGE(Subversion)

  IF(SUBVERSION_FOUND)
    Subversion_WC_INFO(${ROOT_DIR} Project)
    SET(VERSION_REVISION ${Project_WC_REVISION})
  ENDIF(SUBVERSION_FOUND)
ENDIF(EXISTS "${ROOT_DIR}/.svn/")

MESSAGE(STATUS "VERSION=${VERSION_REVISION}")

configure_file(
"${CMAKE_CURRENT_SOURCE_DIR}/version.h.cfg"
"${CMAKE_CURRENT_SOURCE_DIR}/common/version.h"
)

add_subdirectory(libsvr)
add_subdirectory(lua5.4.3)
#add_subdirectory(gateserver)
#add_subdirectory(worldserver)
#add_subdirectory(databaseserver)
#add_subdirectory(gameserver)
#add_subdirectory(chatserver)
#add_subdirectory(databaseserverex)


