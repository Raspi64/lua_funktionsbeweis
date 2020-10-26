// luaexample.cpp
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <string.h>


static void stackDump(lua_State *L) {
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {

            case LUA_TSTRING:  /* strings */
                printf("`%s'", lua_tostring(L, i));
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
        printf("  ");  /* put a separator */
    }
    printf("\n");  /* end the listing */
}

int test(lua_State *L) {
    printf("Hello from C via Lua\n");
    return 0;
}

int main(int argc, char *argv[]) {
    // initialization
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "test", test);

    // execute script
    const char lua_script[] = "test()";
    int load_stat = luaL_loadbuffer(L, lua_script, strlen(lua_script), lua_script);

    stackDump(L);

    lua_pcall(L, 0, 0, 0);

    // cleanup
    lua_close(L);
    return 0;
}


