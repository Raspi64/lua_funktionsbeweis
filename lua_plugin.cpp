#include <cstdlib>
#include "lua_plugin.h"

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

    int lua_io_read(lua_State *_) {
        printf("io.read() is not allowed.\n");
        return 0;
    }

    int lua_io_write(lua_State *_) {
        printf("io.write() is not allowed.\n");
        return 0;
    }

    int lua_print(lua_State *state) {
        char *return_string;
        asprintf(&return_string, "");

        int top = lua_gettop(state);
        for (int i = 1; i <= top; i++) {  /* repeat for each level */
            int t = lua_type(state, i);

            char *string_value;
            switch (t) {
                case LUA_TSTRING:  /* strings */
                    asprintf(&string_value, "%s", lua_tostring(state, i));
                    break;

                case LUA_TBOOLEAN:  /* booleans */
                    asprintf(&string_value, lua_toboolean(state, i) ? "true" : "false");
                    break;

                case LUA_TNUMBER:  /* numbers */
                    asprintf(&string_value, "%g", lua_tonumber(state, i));
                    break;

                default:  /* other values */
                    asprintf(&string_value, "%s", lua_typename(state, t));
                    break;

            }

            char *temp;
            if (i < top) {
                asprintf(&temp, "%s%s\t", return_string, string_value);
            } else {
                asprintf(&temp, "%s%s", return_string, string_value);
            }
            free(return_string);
            free(string_value);
            return_string = temp;
        }

        if (print_function_replacement != nullptr) {
            print_function_replacement(return_string);
        }
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

        lua_register(L, "print", lua_print);
        replace_function_in_table("os", "exit", lua_os_exit); // replaces `os.exit` with `nop_function`
        replace_function_in_table("io", "read", lua_io_read);
        replace_function_in_table("io", "write", lua_io_write);
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
