#include "lua_plugin.h"
#include "lua_plugin_io.h"
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <fcntl.h>

namespace lua_plugin {

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

    static int io_input(lua_State *L) {
        lua_getglobal(L, "io");
        lua_getfield(L, -1, "stdin");
        printf("Top: %d", lua_gettop(L));
        return 1;
    }


    static int io_output(lua_State *L) {
        lua_getglobal(L, "io");
        lua_getfield(L, -1, "stdout");
        printf("Top: %d", lua_gettop(L));
        return 1;
    }

    int in_fd;
    int out_fd;

    pthread_t in_thread;
    pthread_t out_thread;

//    void *input_thread(void *_) {
//        char buf[] = "1234567890";
//        write(in_fd, buf, 10);
//        return nullptr;
//    }


    [[noreturn]] void *input_thread(void *_) {
        char *buf = (char *) malloc(1024);
        while (true) {
            memset(buf, 0, 1024);
            int len = scanf("%s", buf);
            if (len > 0)
                write(in_fd, buf, len);
        }
    }

    [[noreturn]] void *output_thread(void *_) {
        char *buf = (char *) malloc(1024);
        while (true) {
            memset(buf, 0, 1024);
            ssize_t len = read(out_fd, buf, 1024);
            if (len > 0)
                printf("Got: %s", buf);
        }
    }

    void set_stream(lua_State *L, const char *field, FILE *in_stream, const char *reg_index) {
        lua_getglobal(L, "io");
        lua_pushstring(L, field);
        luaL_Stream *stream = newprefile(L);
        stream->f = in_stream;
        stream->closef = &io_noclose;

        lua_pushvalue(L, -1);
        lua_setfield(L, LUA_REGISTRYINDEX, reg_index);  /* add file to registry */

        lua_settable(L, -3);
    }

#define IO_PREFIX    "_IO_"
#define IOPREF_LEN    (sizeof(IO_PREFIX)/sizeof(char) - 1)
#define IO_INPUT    (IO_PREFIX "input")
#define IO_OUTPUT    (IO_PREFIX "output")

    void replace_stdio(lua_State *L) {
        int in_pipefd[2];
        int out_pipefd[2];

        pipe2(in_pipefd, O_RDWR);
        pipe2(out_pipefd, O_RDWR);

        FILE *in_stream = fdopen(in_pipefd[1], "r");
        FILE *out_stream = fdopen(out_pipefd[0], "w");

        in_fd = in_pipefd[0];
        out_fd = out_pipefd[1];

        set_stream(L, "stdin", in_stream, IO_INPUT);
        set_stream(L, "stdout", out_stream, IO_OUTPUT);

        pthread_create(&in_thread, nullptr, input_thread, nullptr);
        pthread_create(&out_thread, nullptr, output_thread, nullptr);
    }
}
