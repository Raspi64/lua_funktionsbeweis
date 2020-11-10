//
// Created by simon on 11/10/20.
//

#ifndef LUA_PLUGIN_LUA_PLUGIN_COLORS_H
#define LUA_PLUGIN_LUA_PLUGIN_COLORS_H

extern "C" {
    #include <lua.h>
}

namespace lua_plugin {
    void init_color_table(struct lua_State *L);
}

#endif //LUA_PLUGIN_LUA_PLUGIN_COLORS_H
