#include <cstdlib>
#include "lua_plugin.h"
#include "lua_plugin_io.h"

namespace lua_plugin {
    static lua_State *L;
    static char *last_error_buffer = nullptr;
    static print_function print_function_replacement;
    static draw_function draw_function_value;

    void set_error_message(const char *error_message);

    int lua_os_exit(lua_State *_) {
        printf("os.exit() is not allowed.\n");
        return 0;
    }

    static void replace_function_in_table(const char *table, const char *field, lua_CFunction function) {
        lua_getglobal(L, table);
        lua_pushstring(L, field);
        lua_pushcfunction(L, function);
        lua_settable(L, -3);
    }

    void plugin_init() {
        L = luaL_newstate();
        luaL_openlibs(L);
        replace_stdio(L);

        replace_function_in_table("os", "exit", lua_os_exit); // replaces `os.exit` with `nop_function`
    }

    void plugin_exit() {
        lua_close(L);
    }


    int exec_script(const char *lua_script) {
        int load_stat = luaL_loadbuffer(L, lua_script, strlen(lua_script), lua_script);

        if (load_stat != LUA_OK) {
            set_error_message(lua_tostring(L, -1));
            return load_stat;
        }

        int exec_stat = lua_pcall(L, 0, 0, 0);

        if (exec_stat != LUA_OK) {
            set_error_message(lua_tostring(L, -1));
        }
        return exec_stat;
    }

    void set_error_message(const char *error_message) {
        char *old_error_buffer = last_error_buffer;
        last_error_buffer = nullptr;
        if (old_error_buffer != nullptr) free(old_error_buffer);
        char *new_error_buffer;

        asprintf(&new_error_buffer, "%s", error_message);
        last_error_buffer = new_error_buffer;
    }

    const char *get_last_error() {
        return last_error_buffer;
    }

    lua_State *get_state() {
        return L;
    }

    void register_print_function(print_function print_func) {
        print_function_replacement = print_func;
    }
}
