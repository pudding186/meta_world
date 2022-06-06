#compile options

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(MSVC)
    set(CMAKE_COMPILER_IS_CLANGCLXX 1)
  else()
    set(CMAKE_COMPILER_IS_CLANGXX 1)
  endif()
elseif (CMAKE_C_COMPILER_ID STREQUAL "Clang")
  if(MSVC)
    set(CMAKE_COMPILER_IS_CLANGCL 1)
  else()
    set(CMAKE_COMPILER_IS_CLANG 1)
  endif()
endif()

if(MSVC)
    if(NOT CMAKE_COMPILER_IS_CLANGCLXX)
        set(CMAKE_COMPILER_IS_MSVCCXX 1)
    endif()
    
    if(NOT CMAKE_COMPILER_IS_CLANGCL)
        set(CMAKE_COMPILER_IS_MSVCC 1)
    endif()
endif()

set(
    VCPKG_PATH
    "$ENV{VCPKG_DIR}"
    )
	
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_MSVCC OR CMAKE_COMPILER_IS_MSVCCXX OR CMAKE_COMPILER_IS_CLANGCL OR CMAKE_COMPILER_IS_CLANGCLXX)
    set(
        VCPKG_ARCH
        "x64-windows-static"
        )

    include_directories(
        include
        ${VCPKG_PATH}/installed/x64-windows-static/include
        )

    foreach(
        var 
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
        )
        
        if(${var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
        endif()
    endforeach()

    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
    #set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")

elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR CMAKE_COMPILER_IS_CLANGXX)
    set(
        VCPKG_ARCH
        "x64-linux"
        )

    include_directories(
	    include
	    ${VCPKG_PATH}/installed/x64-linux/include
        )

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long")

endif()

set(VCPKG_DEBUG_LIB_PATH "${VCPKG_PATH}/installed/${VCPKG_ARCH}/debug/lib")
set(VCPKG_RELEASE_LIB_PATH "${VCPKG_PATH}/installed/${VCPKG_ARCH}/lib")

#MESSAGE(STATUS "VCPKG_DEBUG_LIB_PATH=${VCPKG_DEBUG_LIB_PATH}")
#MESSAGE(STATUS "VCPKG_RELEASE_LIB_PATH=${VCPKG_RELEASE_LIB_PATH}")
macro(target_link_vcpkg_option project_name)

if(CMAKE_COMPILER_IS_MSVCC OR CMAKE_COMPILER_IS_MSVCCXX OR CMAKE_COMPILER_IS_CLANGCL OR CMAKE_COMPILER_IS_CLANGCLXX)

target_link_libraries(${project_name} 
    debug ${VCPKG_DEBUG_LIB_PATH}/fmtd.lib 
    debug ${VCPKG_DEBUG_LIB_PATH}/libcurl-d.lib
    debug ${VCPKG_DEBUG_LIB_PATH}/libssl.lib 
    debug ${VCPKG_DEBUG_LIB_PATH}/libcrypto.lib 
    debug ${VCPKG_DEBUG_LIB_PATH}/pugixml_d.lib
    debug ${VCPKG_DEBUG_LIB_PATH}/mysqlclient.lib
    debug ${VCPKG_DEBUG_LIB_PATH}/zlibd.lib
	debug ${VCPKG_DEBUG_LIB_PATH}/libprotobufd.lib 
    debug ${VCPKG_DEBUG_LIB_PATH}/libprotobuf-lited.lib
    debug ${VCPKG_DEBUG_LIB_PATH}/libprotocd.lib
    optimized ${VCPKG_RELEASE_LIB_PATH}/fmt.lib 
    optimized ${VCPKG_RELEASE_LIB_PATH}/libcurl.lib
    optimized ${VCPKG_RELEASE_LIB_PATH}/libssl.lib 
    optimized ${VCPKG_RELEASE_LIB_PATH}/libcrypto.lib 
    optimized ${VCPKG_RELEASE_LIB_PATH}/pugixml.lib
    optimized ${VCPKG_RELEASE_LIB_PATH}/mysqlclient.lib
    optimized ${VCPKG_RELEASE_LIB_PATH}/zlib.lib
	optimized ${VCPKG_RELEASE_LIB_PATH}/libprotobuf.lib 
    optimized ${VCPKG_RELEASE_LIB_PATH}/libprotobuf-lite.lib
    optimized ${VCPKG_RELEASE_LIB_PATH}/libprotoc.lib
    ws2_32.lib 
    Crypt32.lib 
    Wldap32.lib
    )

set_target_properties(${project_name} PROPERTIES COMPILE_FLAGS "/MP")

elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR CMAKE_COMPILER_IS_CLANGXX)

target_link_libraries(${project_name}
    debug ${VCPKG_DEBUG_LIB_PATH}/libfmtd.a 
    debug ${VCPKG_DEBUG_LIB_PATH}/libcurl-d.a
    debug ${VCPKG_DEBUG_LIB_PATH}/libmysqlclient.a
    debug ${VCPKG_DEBUG_LIB_PATH}/libssl.a 
    debug ${VCPKG_DEBUG_LIB_PATH}/libcrypto.a 
    debug ${VCPKG_DEBUG_LIB_PATH}/libpugixml_d.a
    debug ${VCPKG_DEBUG_LIB_PATH}/libz.a
	debug ${VCPKG_DEBUG_LIB_PATH}/libprotobufd.a 
    debug ${VCPKG_DEBUG_LIB_PATH}/libprotobuf-lited.a
    debug ${VCPKG_DEBUG_LIB_PATH}/libprotocd.a
    optimized ${VCPKG_RELEASE_LIB_PATH}/libfmt.a 
    optimized ${VCPKG_RELEASE_LIB_PATH}/libcurl.a
    optimized ${VCPKG_RELEASE_LIB_PATH}/libmysqlclient.a
    optimized ${VCPKG_RELEASE_LIB_PATH}/libssl.a 
    optimized ${VCPKG_RELEASE_LIB_PATH}/libcrypto.a 
    optimized ${VCPKG_RELEASE_LIB_PATH}/libpugixml.a
    optimized ${VCPKG_RELEASE_LIB_PATH}/libz.a
	optimized ${VCPKG_RELEASE_LIB_PATH}/libprotobuf.a 
    optimized ${VCPKG_RELEASE_LIB_PATH}/libprotobuf-lite.a
    optimized ${VCPKG_RELEASE_LIB_PATH}/libprotoc.a
    dl
    atomic
    pthread
    )

endif()

set_target_properties(${project_name} PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(${project_name} PROPERTIES RELEASE_POSTFIX "")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/../bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/../bin)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
    
endmacro(target_link_vcpkg_option )

macro(target_link_project_option project_name dependlib)
if(CMAKE_COMPILER_IS_MSVCC OR CMAKE_COMPILER_IS_MSVCCXX OR CMAKE_COMPILER_IS_CLANGCL OR CMAKE_COMPILER_IS_CLANGCLXX)
    target_link_libraries(${project_name} 
    debug ${PROJECT_SOURCE_DIR}/../bin/debug/${dependlib}.lib
    optimized ${PROJECT_SOURCE_DIR}/../bin/${RELEASE_TYPE}/${dependlib}.lib
    )
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR CMAKE_COMPILER_IS_CLANGXX)
    target_link_libraries(${project_name} 
    ${PROJECT_SOURCE_DIR}/../bin/lib${dependlib}.a
    )
endif()

	add_dependencies(
    ${PROJECT_NAME}
    ${dependlib}
	)
endmacro(target_link_project_option)

macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)

macro(add_executable_default project_name)

add_executable(
    ${project_name}
    ${RESOURCE}
    ${COMMON}
    ${all_project_files}
)
    
endmacro(add_executable_default)
