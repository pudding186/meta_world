cmake_minimum_required (VERSION 2.80)

project(agentserver)
message(${PROJECT_NAME})

include(../CompileOptions.cmake)
include(../CommonImport.cmake)

set(
    MAIN
    "main.cpp"
    )

set(
    APPLICATION
    "application/agent_server_app.hpp"
    "application/agent_server_app.cpp"
    )

set(
    AGENT
    "agent/agent.hpp"
    "agent/agent.cpp"
    "agent/back_session.hpp"
    "agent/back_session.cpp"
    "agent/front_session.hpp"
    "agent/front_session.cpp"
    "agent/tcp_listener.hpp"
    "agent/tcp_listener.cpp"
    )

set(
    CONFIG
    "config/agent_server_config.hpp"
    "config/agent_server_config.cpp"
    )

set(
    PROTOCOL
    "protocol/agent_protocol.hpp"
    "protocol/agent_protocol.cpp"
    )

set(
    HTTP_TASK
    "http_task/http_task.hpp"
    "http_task/http_task.cpp"
    )

set(all_project_files ${MAIN} ${APPLICATION} ${AGENT} ${CONFIG} ${PROTOCOL} ${HTTP_TASK})

source_group_by_dir(all_project_files)

add_executable_default(${PROJECT_NAME})

target_link_project_option(${PROJECT_NAME} libsvr)

target_link_vcpkg_option(${PROJECT_NAME})
