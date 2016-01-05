#include <windows.h>
#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "FA_lua.h"
#include "H3_function.h"
#include "detour/hookapi.h"
#include "FA_detour.h"

struct FA_DETOUR {
	DWORD Orig;				//原始地址
	DWORD Detour;			//新地址
	DWORD Proxy;			//原代理地址
};

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

	return rtv;
}


struct FA_DETOUR __detours[] = {
	//GlobalEvents,before newturn
	{0x4cd5d0, (DWORD)FA_DoGlobalEvents, 0},
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
		if(__detours[i].Detour == detour)
			return __detours[i].Proxy;
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
		__detours[i].Proxy = (DWORD)HookFunction((void *)__detours[i].Orig, (void *)__detours[i].Detour);
		if(__detours[i].Proxy == 0) {
			FA_Log("FA_Detour_Init 0x%x --> 0x%x Failed!", __detours[i].Orig, __detours[i].Detour);
			return FALSE;
		}
		FA_Log("Detour Org 0x%x --> 0x%x (0x%x) Success!", __detours[i].Orig, __detours[i].Detour, __detours[i].Proxy);
	}

	return TRUE;
}