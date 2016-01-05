/*****************************************************************************
* File Descrip : H3_function Lua Bind
* Create Time  ：20151230
* Author	   ：RedXu
*****************************************************************************/

#include "../FA_def.h"
#include "../FA_lua.h"
#include "../FA_debug.h"
#include "../H3_function.h"

/**
 * Lua Bind H3_BlitPcx16 
 */
static int _H3_BlitPcx16(lua_State *L) {
	const char* pcx = luaL_checkstring(L, 1);
	int p = H3_BlitPcx16(pcx);
	lua_pushinteger(L, p);
	return 1;
}

/**
 * Lua Bind H3_Random
 */
static int _H3_Random(lua_State* L) {
	int min = luaL_checkinteger(L, 1);
	int max = luaL_checkinteger(L, 2);
	if(min > max) {
		FA_Log("_H3_Random min %d max %d wrong!", min, max);
		lua_pushinteger(L, 0);
	}
	else {
		int random = H3_Random(min, max);
		lua_pushinteger(L, random);
	}

	return 1;
}

/**
 * Lua Bind H3_MessageBox
 */
static int _H3_MessageBox(lua_State *L) {
	const char* msg = luaL_checkstring(L, 1);
	int mtype = luaL_checkinteger(L, 2);
	int showtime = luaL_checkinteger(L, 3);
	int rtv = H3_MessageBox(msg, mtype, showtime);
	lua_pushinteger(L, rtv);
	return 1;
}

/**
 * Lua Bind H3_GetPlayerIndex
 */
static int _H3_GetPlayerIndex(lua_State *L) {
	int idx = H3_GetPlayerIndex();
	lua_pushinteger(L, idx);
	return 1;
}

/**
 * Register Homm3.API to LUA
 */
FA_EXPORT int luaopen_Homm3_API(lua_State* L) {
	luaL_checkversion(L);

	luaL_Reg l[] = {
		{"BlitPcx16", _H3_BlitPcx16},
		{"MessageBox", _H3_MessageBox},
		{"Random", _H3_Random},
		{"GetPlayerIndex", _H3_GetPlayerIndex},
		{NULL, NULL},
	};

	luaL_newlib(L, l);

	return 1;
}
