#include <windows.h>
#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "FA_lua.h"
#include "H3_function.h"
#include "detour/hookapi.h"
#include "FA_detour.h"


//declare
static DWORD _FA_Detour_GetProxy(DWORD detour);

/**
 * [Process Global Events (Called before turn,every newday)]
 * @param  ecx [description]
 * @return     [description]
 */
static void* FA_FASTCALL FA_DoGlobalEvents(void* ecx) {
	typedef void* (FA_FASTCALL *F)(void *);
	F proxy = (F)_FA_Detour_GetProxy((DWORD)FA_DoGlobalEvents);
	//assert(proxy != 0);
	void* rtv = proxy(ecx);

	//call script
	if(!luaL_dofile(L, "./script/FA_DoGlobalEvents.lua")) {
		lua_getglobal(L, "FA_DoGlobalEvents");
		//0 args,0 return
		if(lua_pcall(L, 0, 0, 0)) {
			FA_Log("lua call FA_DoGlobalEvents Failed! [%s]", lua_tostring(L, -1));
		}
	}
	else {
		FA_Log("loading FA_DoGlobalEvents.lua Failed! [%s]", lua_tostring(L, -1));
	}

	//
	/*
	int msz = H3_GetMapSize();
	int i,j;
	for(i = 0; i < msz; i++)
	for(j = 0; j < msz; j++)
	{
	struct H3_MapItem* item = H3_GetMapItem(i, j, 0);
	//if(item->otype != 5)
	//	continue;
	//FA_DumpTxt(item, sizeof(*item));
	FA_Log("item(%d,%d,%p) river=%d, type=%d",i,j,item, item->river, item->type);
	}
	*/

	return rtv;
}

/**
 * [CheckReg Call]
 * @return  [1~6 error >7 success]
 */
static int FA_CDECL FA_CheckReg(void) {
	return 7;
}


struct FA_Mod __detours[] = {
	//GlobalEvents,before newturn
	{FA_MOD_TYPE_DETOUR, 0x4cd5d0, (DWORD)FA_DoGlobalEvents, 0},
	//check reg
	{FA_MOD_TYPE_DETOUR, 0x50c380, (DWORD)FA_CheckReg, 0},
};


/**
 * [获取函数代理地址]
 * [注意注册的函数不能重复]
 * @param  detour [新地址]
 * @return        [代理地址]
 */
static DWORD _FA_Detour_GetProxy(DWORD detour) {
	int i,n;
	n = FA_ARRAYSIZE(__detours);
	for(i=0; i<n; i++) {
		if(__detours[i].Type == FA_MOD_TYPE_DETOUR && __detours[i].Detour == detour)
			return __detours[i].U.Proxy;
	}

	return 0;
}

/**
 * 劫持HoMM3的函数
 * @return  [TRUE-成功 FALSE-失败]
 */
BOOL FA_Detour_Init(void) {
	int i,n;
	n = FA_ARRAYSIZE(__detours);
	for(i=0; i<n; i++) {
		__detours[i].U.Proxy = (DWORD)HookFunction((void *)__detours[i].Orig, (void *)__detours[i].Detour);
		if(__detours[i].U.Proxy == 0) {
			FA_Log("FA_Detour_Init 0x%x --> 0x%x Failed!", __detours[i].Orig, __detours[i].Detour);
			return FALSE;
		}
		FA_Log("Detour Org 0x%x --> 0x%x (0x%x) Success!", __detours[i].Orig, __detours[i].Detour, __detours[i].U.Proxy);
	}

	return TRUE;
}
