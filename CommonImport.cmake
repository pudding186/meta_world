set (
    COMMON
    "../common/util/app_util.hpp"
    "../common/util/app_util.cpp"
    "../common/app/svr_app.hpp"
    "../common/guid/guid_system.hpp"
    "../common/guid/guid_system.cpp"
	"../common/logic_define/server_common_define.hpp"
    "../common/logic_define/server_common_define.cpp"
    "../common/object/object.hpp"
    "../common/fsm/fsm.hpp"
    "../common/timer/timer_system.hpp"
    "../common/timer/timer_system.cpp"
    "../common/log/log_system.hpp"
    "../common/log/log_system.cpp"
    "../common/net/net_system.hpp"
    "../common/net/net_system.cpp"
    "../common/net/session.hpp"
    "../common/net/session.cpp"
    "../common/net/pkg_header.hpp"
    "../common/http/http_system.hpp"
    "../common/http/http_system.cpp"
    "../common/database/database_system.hpp"
    "../common/database/database_system.cpp"
    "../common/custom/custom_var.hpp"
    "../common/custom/custom_var.cpp"
    "../common/protocol/protocol_system.hpp"
	"../common/protocol/DataDefine.hpp"
    "../common/protocol/DataDefine.cpp"
    "../common/protocol/StructDef.hpp"
    "../common/protocol/StructDef.cpp"
    "../common/protocol/ClientCS.hpp"
    "../common/protocol/ClientCS.cpp"
    "../common/protocol/ClientGS.hpp"
    "../common/protocol/ClientGS.cpp"
    "../common/protocol/ClientWS.hpp"
    "../common/protocol/ClientWS.cpp"
    "../common/protocol/FightData.hpp"
    "../common/protocol/FightData.cpp"
    "../common/protocol/Global.hpp"
    "../common/protocol/Global.cpp"
    "../common/protocol/ErrorSystem.hpp"
    "../common/protocol/ErrorSystem.cpp"
    "../common/dbtable/dbtable_system.hpp"
    "../common/dbtable/mt_zone.hpp"
    "../common/dbtable/mt_zone.cpp"
    "../common/dbtable/mt_user.hpp"
    "../common/dbtable/mt_user.cpp"
    "../common/dbtable/mt_role.hpp"
    "../common/dbtable/mt_role.cpp"
    "../common/dbtable/mt_ranklist.hpp"
    "../common/dbtable/mt_ranklist.cpp"
    "../common/dbtable/mt_pasture.hpp"
    "../common/dbtable/mt_pasture.cpp"
    "../common/dbtable/mt_mail.hpp"
    "../common/dbtable/mt_mail.cpp"
    "../common/dbtable/mt_ladder.hpp"
    "../common/dbtable/mt_ladder.cpp"
    "../common/dbtable/mt_guild_member.hpp"
    "../common/dbtable/mt_guild_member.cpp"
    "../common/dbtable/mt_guild_applicant.hpp"
    "../common/dbtable/mt_guild_applicant.cpp"
    "../common/dbtable/mt_guild.hpp"
    "../common/dbtable/mt_guild.cpp"
    "../common/dbtable/mt_forbidtalk.hpp"
    "../common/dbtable/mt_forbidtalk.cpp"
    "../common/dbtable/mt_forbid.hpp"
    "../common/dbtable/mt_forbid.cpp"
    "../common/dbtable/mt_dbvar.hpp"
    "../common/dbtable/mt_dbvar.cpp"
    "../common/dbtable/mt_contact.hpp"
    "../common/dbtable/mt_contact.cpp"
    "../common/dbtable/mt_billlog2.hpp"
    "../common/dbtable/mt_billlog2.cpp"
    "../common/dbtable/mt_billlog.hpp"
    "../common/dbtable/mt_billlog.cpp"
    "../common/dbtable/mt_auction_object.hpp"
    "../common/dbtable/mt_auction_object.cpp"
    "../common/dbtable/mt_auction_cookie.hpp"
    "../common/dbtable/mt_auction_cookie.cpp"
    )

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG OR CMAKE_COMPILER_IS_CLANGXX)
list(APPEND COMMON "../common/util/kbhit.c")
list(APPEND COMMON "../common/util/kbhit.h")
list(APPEND COMMON "../common/util/linux_util.c")

set(
    RESOURCE
    "../common/version.h"
)

elseif(CMAKE_COMPILER_IS_MSVCC OR CMAKE_COMPILER_IS_MSVCCXX OR CMAKE_COMPILER_IS_CLANGCL OR CMAKE_COMPILER_IS_CLANGCLXX)
list(APPEND COMMON "../common/minidump/dump_system.hpp")
list(APPEND COMMON "../common/minidump/dump_system.cpp")
list(APPEND COMMON "../common/util/windows_util.c")

configure_file(
    "${CMAKE_SOURCE_DIR}/version.rc.cfg"
    "${PROJECT_SOURCE_DIR}/version.rc"
    )

set(
    RESOURCE
    "version.rc"
    "../common/version.h"
)

source_group(
    "resource"
    FILES
    ${RESOURCE}
)

endif()

source_group(
    TREE
    ${PROJECT_SOURCE_DIR}/../
    FILES
    ${COMMON}
)

include_directories(
    .
    ..
    ${VCPKG_PATH}/installed/${VCPKG_ARCH}/include
    ../libsvr/include
	../lua-5.4.4/src
    ../common
    ../common/app
    ../common/util
    ../common/minidump
    ../common/timer
    ../common/log
    ../common/net
    ../common/database
    ../common/guid
    ../common/config
    ../common/object
    ../common/fsm
    ../common/protocol
    ../common/http
    ../common/dbtable
)