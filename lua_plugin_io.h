#ifndef LUA_PLUGIN_LUA_PLUGIN_IO_H
#define LUA_PLUGIN_LUA_PLUGIN_IO_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace lua_plugin {
    void replace_stdio(lua_State *L);
}


#endif //LUA_PLUGIN_LUA_PLUGIN_IO_H
