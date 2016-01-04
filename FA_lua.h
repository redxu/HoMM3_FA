#ifndef __FA_LUA__H__
#define __FA_LUA__H__

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"


extern lua_State* L;

/** init **/
void FA_lua_init(void);

/** close **/
void FA_lua_close(void);


#endif
