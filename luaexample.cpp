// luaexample.cpp
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <cstring>


void exec_script(lua_State *L, const char *lua_script);

void replace_function_in_table(lua_State *L, const char *table, const char *field, lua_CFunction function);

int stackDump(lua_State *L) {
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

int do_math_things(lua_State *L) {
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

int nop_function(lua_State *L) {
    printf("caught function!!!\n");
    return 0;
}

int main(int argc, char *argv[]) {
    // initialization
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "print", stackDump);
    replace_function_in_table(L, "os", "exit", nop_function); // replaces `os.exit` with `nop_function`

    // register custom function
    lua_register(L, "do_math", do_math_things);

    printf("Erfolgreicher Aufruf:\n");
    {
        const char lua_script[] = "local x = 15\n"
                                  "add, sub, mul, div = do_math(x, 5)\n"
                                  "print(x .. ' + 5 = ' .. add)\n"
                                  "print(x .. ' - 5 = ' .. sub)\n"
                                  "print(x .. ' * 5 = ' .. mul)\n"
                                  "print(x .. ' / 5 = ' .. div)\n"
                                  "os.exit()\n";
        // load script on top of stack
        exec_script(L, lua_script);
    }
    printf("\n\n\nFehlende Variable:\n");

    {
        const char lua_script[] = "do_math(x, 5)";
        exec_script(L, lua_script);
    }

    printf("\n\n\nFalscher Variablentyp:\n");
    {
        const char lua_script[] = "do_math('aaa', 5)";
        exec_script(L, lua_script);
    }
    // show stack content
//    stackDump(L);

    // cleanup
    lua_close(L);
    return 0;
}

void replace_function_in_table(lua_State *L, const char *table, const char *field, lua_CFunction function) {
    lua_getglobal(L, table);
    lua_pushstring(L, field);
    lua_pushcfunction(L, function);
    lua_settable(L, -3);
}

void exec_script(lua_State *L, const char *lua_script) {
    int load_stat = luaL_loadbuffer(L, lua_script, strlen(lua_script), lua_script);

    if (!load_stat) {
        printf("Load: OK\n");
    } else {
        printf("Load error: %d\n", load_stat);
    }

    int exec_stat = lua_pcall(L, 0, 0, 0);

    if (!exec_stat) {
        printf("Exec: OK\n");
    } else {
        printf("Exec error: %d\n\n", exec_stat);
    }

    while (exec_stat && lua_gettop(L)) {
        printf("Stack size: %d\n", lua_gettop(L));
        printf("Error message: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        exec_stat = lua_pcall(L, 0, 0, 0);
    }
}
