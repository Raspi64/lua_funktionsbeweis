#include "lua_plugin_colors.h"

namespace lua_plugin {
    void push_color_table(lua_State *L, const char *name, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        lua_createtable(L, 0, 4);

        lua_pushnumber(L, r);
        lua_setfield(L, -2, "r");

        lua_pushnumber(L, g);
        lua_setfield(L, -2, "g");

        lua_pushnumber(L, b);
        lua_setfield(L, -2, "b");

        lua_pushnumber(L, a);
        lua_setfield(L, -2, "a");

        lua_setfield(L, -2, name);
    }

    void init_color_table(struct lua_State *L) {
        lua_createtable(L, 0, 16);
        push_color_table(L, "white", 0xDD, 0xDD, 0xDD, 0xFF);
        push_color_table(L, "orange", 0xDB, 0x7D, 0x3E, 0xFF);
        push_color_table(L, "magenta", 0xB3, 0x50, 0xBC, 0xFF);
        push_color_table(L, "light_blue", 0x6B, 0x8A, 0xC9, 0xFF);
        push_color_table(L, "yellow", 0xB1, 0xA6, 0x27, 0xFF);
        push_color_table(L, "lime", 0x41, 0xAE, 0x38, 0xFF);
        push_color_table(L, "pink", 0xD0, 0x84, 0x99, 0xFF);
        push_color_table(L, "gray", 0x40, 0x40, 0x40, 0xFF);
        push_color_table(L, "light_gray", 0x9A, 0xA1, 0xA1, 0xFF);
        push_color_table(L, "cyan", 0x2E, 0x6E, 0x89, 0xFF);
        push_color_table(L, "purple", 0x7E, 0x3D, 0xB5, 0xFF);
        push_color_table(L, "blue", 0x2E, 0x38, 0x8D, 0xFF);
        push_color_table(L, "brown", 0x4F, 0x32, 0x1F, 0xFF);
        push_color_table(L, "green", 0x35, 0x46, 0x1B, 0xFF);
        push_color_table(L, "red", 0x96, 0x34, 0x30, 0xFF);
        push_color_table(L, "black", 0x19, 0x16, 0x16, 0xFF);
        lua_setglobal(L, "colors");
    }
}
