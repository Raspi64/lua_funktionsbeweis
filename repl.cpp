
extern "C" {
#include <lua.h>
}

#include <cstdio>
#include <cstdlib>
#include "lua_plugin.h"
#include "lua_plugin_colors.h"

static int call_lua_function(lua_State *L);

static int stackDump(lua_State *L);

static int do_math_things(lua_State *L);

void repl();

int main(int argc, char *argv[]) {
    lua_plugin::plugin_init();
    lua_State *L = lua_plugin::get_state();

    lua_plugin::init_color_table(L);

    lua_register(L, "do_math", do_math_things);
    lua_register(L, "stackdump", stackDump);
    lua_register(L, "call_lua_function", call_lua_function);

    repl();

    lua_plugin::plugin_exit();
    return 0;
}

void my_print(const char *string) {
    printf("< %s\n", string);
}

void repl() {
    const char *exit = "exit\n";
    char *user_input = nullptr;

    lua_plugin::register_print_function(my_print);
    while (true) {
        printf("> ");
        size_t len = 0;
        getline(&user_input, &len, stdin);
        if (strcmp(exit, user_input) == 0) {
            free(user_input);
            printf("Exiting...\n");
            break;
        }
        if (lua_plugin::exec_script(user_input) != LUA_OK) {
            printf("Error: %s\n", lua_plugin::get_last_error());
        }
        free(user_input);
    }
}


static int stackDump(lua_State *L) {
    printf("Stackdump: ");
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

            case LUA_TSTRING:  /* strings */
                printf("'%s'", lua_tostring(L, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;

            case LUA_TNUMBER:  /* numbers */
                printf("%g", lua_tonumber(L, i));
                break;

            default:  /* other values */
                printf("%s", lua_typename(L, t));
                break;

        }
        if (i < top) {
            printf("  ");  /* put a separator */
        }
    }
    printf("\n");  /* end the listing */
    return 0;
}

static int call_lua_function(lua_State *L) {

    const char *function_name = "tonumber";
    const char *number_input_string = "123234";
    int number_input = 123234;


    printf("Calling %s with parameter %d\n", function_name, number_input);

    lua_getglobal(L, function_name);  /* function to be called */
    lua_pushstring(L, number_input_string);   /* push 1st argument */
    /* do the call (1 arguments, 1 result) */
    if (lua_pcall(L, 1, 1, 0) != 0) {
        printf("error running function '%s': %s\n", function_name, lua_tostring(L, -1));
    }
    /* retrieve result */
    if (!lua_isnumber(L, -1)) {
        printf("function '%s' must return a number\n", function_name);
    }
    int z = lua_tonumber(L, -1);
    lua_pop(L, 1);  /* pop returned value */
    if (z != 123234) {
        printf("return should be %s, but is %d\n", number_input_string, z);
    } else {
        printf("return should be %s, and is %d\n", number_input_string, z);
    }
    return 0;
}


static int do_math_things(lua_State *L) {
    if (lua_gettop(L) != 2) {
        return luaL_error(L, "expecting exactly 2 numbers");
    }
    if (!lua_isnumber(L, 1)) {
        return luaL_error(L, "expecting the first parameter to be a number");
    }
    if (!lua_isnumber(L, 2)) {
        return luaL_error(L, "expecting the second parameter to be a number");
    }
    int a = lua_tointeger(L, 1);
    int b = lua_tointeger(L, 2);
//    printf("%d,%d\n",a,b);
    lua_pushinteger(L, a + b);
    lua_pushinteger(L, a - b);
    lua_pushinteger(L, a * b);
    lua_pushinteger(L, a / b);
    return 4;
}
