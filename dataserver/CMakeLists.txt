cmake_minimum_required (VERSION 2.80)

project(dataserver)
message(${PROJECT_NAME})

include(../CompileOptions.cmake)
include(../CommonImport.cmake)

set(
    MAIN
    "main.cpp"
    )

set(
    APPLICATION
    "application/data_server_app.hpp"
    "application/data_server_app.cpp"
    )

set(
    CONFIG
    "config/data_server_config.hpp"
    "config/data_server_config.cpp"
    )

set(
    DATABASE
    "database/database_table_system.hpp"
    "database/database_table_system.cpp"
)

set(
    NET
    "net/control_server.hpp"
    "net/control_server.cpp"
    "net/recharge_server.hpp"
    "net/recharge_server.cpp"
    "net/world_server.hpp"
    "net/world_server.cpp"
)

set(
    PROTOCOL
    "protocol/database_protocol.hpp"
    "protocol/database_protocol.cpp"
    "protocol/database_world_protocol.hpp"
    "protocol/database_world_protocol.cpp"
    "protocol/global_protocol.hpp"
    "protocol/global_protocol.cpp"
)

set(all_project_files ${MAIN} ${APPLICATION} ${CONFIG} ${DATABASE} ${NET} ${PROTOCOL})

source_group_by_dir(all_project_files)

add_executable_default(${PROJECT_NAME})

target_link_project_option(${PROJECT_NAME} libsvr)

target_link_vcpkg_option(${PROJECT_NAME})
