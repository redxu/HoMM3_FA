/*****************************************************************************
* File Descrip : LoadBar Loading Screen
* Create Time  ：20160113
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "FA_lua.h"
#include "FA_mod.h"

/**
 * FA LoadBar加载画面
 */
static int FA_STDCALL FA_LoadBar(void) {
	if(!luaL_dofile(L, "./script/FA_LoadBar.lua")) {
		lua_getglobal(L, "FA_LoadBar");
		//0 args,1 return
		if(lua_pcall(L, 0, 1, 0)) {
			FA_Log("lua call FA_LoadBar Failed! [%s]", lua_tostring(L, -1));
		}
		int ukp = lua_tointeger(L, -1);
		lua_pop(L, 1);
		return ukp;
	}
	else {
		FA_Log("loading FA_LoadBar.lua Failed! [%s]", lua_tostring(L, -1));
	}

	return 0;
}

//local mods for heroskill
static struct FA_mod __mods[] = {
	//NewGame->LoadBar
	{FA_MOD_TYPE_CALL, 0x004ed9ea, (DWORD)FA_LoadBar, 10},
	//RestartGame->LoadBar
	{FA_MOD_TYPE_CALL, 0x004ef854, (DWORD)FA_LoadBar, 10},
};


/**
 * [FA_HeroSkill_Init]
 */
void FA_Loadbar_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);
}
