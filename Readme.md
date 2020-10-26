# Lua Funktionsbeweis

## Anwendung


### 1. Neuen State erstellen
```c
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
```


### 2. Funktion mit diesem Header erstellen
```c
int test(lua_State *L) {
    printf("Hello from C via Lua\n");
    return 0;
}
```


### 3. Funktion registrieren
(String ist der Funktionsname in Lua)
```c
    lua_register(L, "test", test);
```

### 4. Script-Puffer in Lua auf das Stack legen 
```c
    const char lua_script[] = "test()";
    int load_stat = luaL_loadbuffer(L, lua_script, strlen(lua_script), lua_script);
```

### 5. Code, der oben auf auf dem Stack liegt ausfuehren 
```c
    lua_pcall(L, 0, 0, 0);
```

### 6. Aufraeumen
```c
    // cleanup
    lua_close(L);
    return 0;
```
