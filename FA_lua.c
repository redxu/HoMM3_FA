#include "FA_lua.h"


lua_State* L = NULL;

/**
 * Init Lua Env
 */
void FA_lua_init(void) {
	L = luaL_newstate();
	luaL_openlibs(L);

	//set lua cpath
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "cpath");
	const char* lua_cpath = lua_tostring(L, -1);
	char fa_cpath[1024];
	sprintf(fa_cpath, "%s;%s", lua_cpath, "Homm3_FA.dll");
	lua_pop(L, 1);			// pop lua_tostring
	lua_pushstring(L, fa_cpath);
	lua_setfield(L, -2, "cpath");
	lua_pop(L, 1);
}

/**
 * Close Lua
 */
void FA_lua_close(void) {
	lua_close(L);
}
