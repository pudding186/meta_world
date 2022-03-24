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
    "${CMAKE_SOURCE_DIR}/../vcpkg"
    )

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


    #target_link_libraries(${PROJECT_NAME} dl)
    #target_link_libraries(${PROJECT_NAME} pthread)

    # Update if necessary
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -std=gnu++11")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -std=gnu11")
		#target_link_libraries(${PROJECT_NAME} atomic)
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -std=gnu++11 -mcx16")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -std=gnu11 -mcx16")
    endif()
endif()
