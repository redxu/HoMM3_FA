/*****************************************************************************
* File Descrip : H3_log Lua Bind
* Create Time  ：20151230
* Author	   ：RedXu
*****************************************************************************/

#include "../FA_def.h"
#include "../FA_lua.h"
#include "../FA_log.h"
#include "../H3_function.h"

/**
 * 不会写
 * @param  L [description]
 * @return   [description]
 */
static int _FA_log(lua_State* L) {
	const char* fmt = lua_tostring(L, 1);

	return 1;
}