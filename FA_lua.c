#include "FA_lua.h"


lua_State* L = NULL;

/**
 * 初始化LUA库
 */
void FA_lua_init(void) {
	L = luaL_newstate();
	luaL_openlibs(L);
}

/**
 * 关闭LUA库
 */
void FA_lua_close(void) {
	lua_close(L);
}
