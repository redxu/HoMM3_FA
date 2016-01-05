#include "patch.h"
#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "FA_lua.h"
#include "H3_Function.h"
#include "FA_mod.h"


struct FA_MOD {
	DWORD Orig;			/**原始地址**/
	DWORD Detour;		/**新函数地址**/
	DWORD Type;			/**补丁方式**/
	DWORD Size;			/**补丁大小**/
};


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

//for test
static void* FA_FASTCALL TestClick(void* ecx) {
	int idx = H3_GetPlayerIndex();
	FA_Log("current player=%d",idx);
	struct H3_Player* player = H3_GetPlayer(idx);
	FA_Log("%p player->curhero=%d tophero=%d", player, player->curhero, player->tophero);
	FA_Log("player->name=%s %p",player->name,player->name);
	struct H3_Hero* hero = H3_GetHero(player->curhero);
	FA_Log("%p hero->name=%s", hero, hero->name);
	FA_Log("hero->movement=%d/%d",hero->curmovement,hero->maxmovement);
	FA_Log("hero->adpk = %d %d %d %d",hero->adpk[0], hero->adpk[1], hero->adpk[2], hero->adpk[3]);

	FA_DumpTxt((DWORD)hero, sizeof(struct H3_Hero));

	typedef void* (FA_FASTCALL* F)(void *);
	F proxy = (F)0x4de980; 
	return proxy(ecx);
}

static struct FA_MOD __mods[] = {
	//NewGame->LoadBar
	{0x004ed9ea, (DWORD)FA_LoadBar, FA_MOD_TYPE_CALL, 10},
	//RestartGame->LoadBar
	{0x004ef854, (DWORD)FA_LoadBar, FA_MOD_TYPE_CALL, 10},
	//test mod
	{0x004e1afa, (DWORD)TestClick, FA_MOD_TYPE_CALL, 5},
};

/**
 * 修改HoMM3的流程
 * @return  [TRUE-成功 FALSE-失败]
 */
BOOL FA_Mod_Init(void) {
	int i,n;
	unsigned char patch[256];
	
	n = FA_ARRAYSIZE(__mods);
	for(i=0; i<n; i++) {
		struct FA_MOD* mod = &__mods[i];
		memset(patch, 0x90, sizeof(patch));
		if(mod->Type == FA_MOD_TYPE_CALL) {
			DWORD jmpoff = mod->Detour - mod->Orig - 5;
			patch[0] = 0xe8;
			memcpy(patch+1, &jmpoff, 4);
			if(!PatchCode((PVOID)mod->Orig, patch, mod->Size)) {
				FA_Log("patch 0x%x Failed!", mod->Orig);
				return FALSE;
			}
		}
		else {
			FA_Log("unhandle mod type %d", mod->Type);
			return FALSE;
		}
	}

	return TRUE;
}

