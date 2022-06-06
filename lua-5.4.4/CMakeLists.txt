cmake_minimum_required(VERSION 3.6)

GET_FILENAME_COMPONENT(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})
message(${PROJECT_NAME})

include(../CompileOptions.cmake)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src ALL_SRC)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)

# remove the lua.c/luac.c from teh all file lists
SET(LIB_SRC ${ALL_SRC})
LIST(REMOVE_ITEM LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)

SET_SOURCE_FILES_PROPERTIES(${LIB_SRC} PROPERTIES LANGUAGE CXX)

if(CMAKE_COMPILER_IS_MSVCC OR CMAKE_COMPILER_IS_MSVCCXX)
add_definitions(-w)
elseif(CMAKE_COMPILER_IS_CLANG OR CMAKE_COMPILER_IS_CLANGXX)
add_definitions(-w)
endif()

ADD_LIBRARY(${PROJECT_NAME} STATIC ${LIB_SRC})

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

#ADD_EXECUTABLE(luaexec ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c)
#target_link_libraries(luaexec ${PROJECT_NAME})

#add_executable(luac ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)
#target_link_libraries(luac ${PROJECT_NAME})

#add_executable(lua_test ${CMAKE_CURRENT_SOURCE_DIR}/test/lua_test.cpp)
#target_link_libraries(lua_test ${PROJECT_NAME})