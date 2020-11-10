#include "lua_plugin_io.h"

static luaL_Stream *newprefile(lua_State *L) {
    auto *p = (luaL_Stream *) lua_newuserdatauv(L, sizeof(luaL_Stream), 0);
    p->closef = nullptr;  /* mark file handle as 'closed' */
    luaL_setmetatable(L, LUA_FILEHANDLE);
    return p;
}

static int io_noclose(lua_State *L) {
    luaL_Stream *p = ((luaL_Stream *) luaL_checkudata(L, 1, LUA_FILEHANDLE));
    p->closef = &io_noclose;  /* keep file opened */
    luaL_pushfail(L);
    lua_pushliteral(L, "cannot close standard file");
    return 2;
}

static void createstdfile(lua_State *L, FILE *f, const char *k, const char *fname) {
    luaL_Stream *p = newprefile(L);
    p->f = f;
    p->closef = &io_noclose;
    if (k != nullptr) {
        lua_pushvalue(L, -1);
        lua_setfield(L, LUA_REGISTRYINDEX, k);  /* add file to registry */
    }
    lua_setfield(L, -2, fname);  /* add file to module */
}

#define IO_PREFIX    "_IO_"
#define IOPREF_LEN    (sizeof(IO_PREFIX)/sizeof(char) - 1)
#define IO_INPUT    (IO_PREFIX "input")
#define IO_OUTPUT    (IO_PREFIX "output")

void replace_stdio(lua_State *L) {
    char *out_buffer;
    size_t out_buffer_len;
    FILE *out_stream = open_memstream(&out_buffer, &out_buffer_len);

    char *err_buffer;
    size_t err_buffer_len;
    FILE *err_stream = open_memstream(&err_buffer, &err_buffer_len);

    lua_getglobal(L, "io");
    createstdfile(L, stdin, IO_INPUT, "stdin");
    createstdfile(L, out_stream, IO_OUTPUT, "stdout");
    createstdfile(L, err_stream, nullptr, "stderr");
}
