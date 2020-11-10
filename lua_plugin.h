//
// Created by simon on 11/9/20.
//

#ifndef LUATEST_LUA_PLUGIN_H
#define LUATEST_LUA_PLUGIN_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <cstring>

namespace lua_plugin {
    typedef void (*print_function)(const char *);

    typedef void (*draw_function)(int x, int y, int r, int g, int b, int alpha, int size);


    namespace MouseEvent {
        enum Type {
            Press,
            Release,
            Move
        };
    }

    namespace KeyboardEvent {
        enum Type {
            Press,
            Release
        };
    }


    void plugin_init();

    void plugin_exit();

    int exec_script(const char *lua_script);

    const char *get_last_error();

    lua_State *get_state();

    void register_print_function(print_function print_function_replacement);

    void register_draw_function(draw_function draw_function_replacement);

    void mouse_event(int x, int y, MouseEvent::Type type);

    void keyboard_event(char key, KeyboardEvent::Type type);
}

#endif //LUATEST_LUA_PLUGIN_H
