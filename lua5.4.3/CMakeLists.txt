cmake_minimum_required (VERSION 2.80)

project (lua5.4.3)

include(../CompileOptions.cmake)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)

set (
    HEADER
    "./lopcodes.h"
    "./ltm.h"
    "./lua.h"
    "./luaconf.h"
    "./lualib.h"
    "./lundump.h"
    "./lvm.h"
    "./lzio.h"
    "./llex.h"
    "./llimits.h"
    "./lmem.h"
    "./lopnames.h"
    "./lparser.h"
    "./lprefix.h"
    "./lstate.h"
    "./lstring.h"
    "./ltable.h"
    "./lapi.h"
    "./lauxlib.h"
    "./lcode.h"
    "./lctype.h"
    "./ldebug.h"
    "./ldo.h"
    "./lfunc.h"
    "./lgc.h"
    "./ljumptab.h"
    "./strbuf.h"
    "./lua.hpp"    
)

set (
    SOURCE
    "./lstrlib.cpp"
    "./lgc.cpp"
    "./loadlib.cpp"
    "./lparser.cpp"
    "./lcode.cpp"
    "./lcorolib.cpp"
    "./ldblib.cpp"
    "./luac.cpp"
    "./ltable.cpp"
    "./lua.cpp"
    "./lundump.cpp"
    "./lutf8lib.cpp"
    "./lvm.cpp"
    "./lzio.cpp"
    "./llex.cpp"
    "./lmathlib.cpp"
    "./lmem.cpp"
    "./lobject.cpp"
    "./lopcodes.cpp"
    "./loslib.cpp"
    "./lstate.cpp"
    "./lstring.cpp"
    "./ltablib.cpp"
    "./ltm.cpp"
    "./lapi.cpp"
    "./lauxlib.cpp"
    "./lbaselib.cpp"
    "./lctype.cpp"
    "./ldebug.cpp"
    "./ldo.cpp"
    "./ldump.cpp"
    "./lfunc.cpp"
    "./linit.cpp"
    "./liolib.cpp"
    "./strbuf.cpp"
    "./lua_cjson.cpp"
    "./luaudata.cpp"
)

source_group (
    header
    FILES 
    ${HEADER} 
)

source_group (
    source
    FILES 
    ${SOURCE} 
)

add_library(${PROJECT_NAME} STATIC ${HEADER} ${SOURCE})

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
