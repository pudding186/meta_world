#define lstrlib_c
#define LUA_LIB

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"

static int udata_isnull(lua_State *L)
{
    if (lua_isnil(L, -1))
    {
        lua_pushboolean(L, 1);
        return 1;
    }

    if (!lua_isuserdata(L, -1))
    {
        lua_pushboolean(L, 0);
        return 1;
    }
    
    void **udata = (void **)lua_touserdata(L, -1);
    lua_pushboolean(L, (*udata) ? 0 : 1);
    return 1;
}

static const luaL_Reg udatalib[] = 
{
    { "isnull", udata_isnull },
    { NULL, NULL }
};

LUAMOD_API int luaopen_udata(lua_State *L) 
{
    luaL_newlib(L, udatalib);
    return 1;
}
