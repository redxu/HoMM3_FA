/*****************************************************************************
* File Descrip : FA_log Lua Bind
* Create Time  ：20151230
* Author	   ：RedXu
*****************************************************************************/

#include "../FA_def.h"
#include "../FA_lua.h"
#include "../FA_debug.h"

/**
 * Lua Bind FA_log
 */
static int _FA_Log(lua_State* L) {
	const char* log = luaL_checkstring(L, 1);
	FA_Log(log);
	return 0;
}

/**
 * Register Homm3.DEBUG to LUA
 */
FA_EXPORT int luaopen_Homm3_DEBUG(lua_State* L) {
	luaL_checkversion(L);

	luaL_Reg l[] = {
		{"Log", _FA_Log},
		{NULL, NULL},
	};

	luaL_newlib(L, l);

	return 1;
}
