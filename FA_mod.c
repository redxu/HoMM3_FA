/*****************************************************************************
* File Descrip : FA Mod Flow
* Create Time  ：20160113
* Author	   ：RedXu
*****************************************************************************/

#include "patch.h"
#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "FA_mod.h"


/**
 * mod_queue
 */
static struct FA_Mod_Q __mq;


//extern from FA_Heroskill
extern void FA_HeroSkill_Init(void);
//extern from FA_loadbar
extern void FA_Loadbar_Init(void);
//extern from FA_DlgHeroInfo
extern void FA_DlgHeroInfo_Init(void);
//extern from FA_Lod
extern void FA_Lod_Init(void);
//extern from FA_SSkill
extern void FA_SSkill_Init(void);
//extern from FA_EyeOfMagi
extern void FA_EyeOfMagi_Init(void);
//extern from FA_Artifact
extern void FA_Artifact_Init(void);
//extern from FA_HeroExp
extern void FA_HeroExp_Init(void);

/**
 * 修改HoMM3的流程
 * @return  [TRUE-成功 FALSE-失败]
 */
BOOL FA_Mod_Init(void) {
	int i;
	unsigned char patch[1024];

	memset(&__mq, 0, sizeof(__mq));
	FA_Loadbar_Init();
	FA_HeroSkill_Init();
	FA_DlgHeroInfo_Init();
	FA_Lod_Init();
	FA_SSkill_Init();
	FA_EyeOfMagi_Init();
	FA_Artifact_Init();
	FA_HeroExp_Init();

	for(i=0; i<__mq.sz; i++) {
		struct FA_Mod* mod = &__mq.mods[i];
		memset(patch, 0x90, sizeof(patch));
		if(mod->Type == FA_MOD_TYPE_CALL) {
			DWORD jmpoff = mod->Detour - mod->Orig - 5;
			patch[0] = 0xe8;
			memcpy(patch+1, &jmpoff, 4);
			if(!PatchCode((PVOID)mod->Orig, patch, mod->U.Size)) {
				FA_Log("patch 0x%x Failed!", mod->Orig);
				return FALSE;
			}
		}
		else if(mod->Type == FA_MOD_TYPE_WRITE) {
			if(!PatchCode((PVOID)mod->Orig, (PVOID)mod->Detour, mod->U.Size)) {
				FA_Log("patch 0x%x Failed!", mod->Orig);
				return FALSE;
			}
		}
		else if(mod->Type == FA_MOD_TYPE_NOP) {
			if(!PatchCode((PVOID)mod->Orig, patch, mod->U.Size)) {
				FA_Log("patch 0x%x Failed!", mod->Orig);
				return FALSE;
			}
		}
		else if(mod->Type == FA_MOD_TYPE_BYTE) {
			if(!PatchCode((PVOID)mod->Orig, (BYTE *)&mod->Detour, mod->U.Size)) {
				FA_Log("patch 0x%x Failed!", mod->Orig);
				return FALSE;
			}
		}
		else if(mod->Type == FA_MOD_TYPE_DETOUR) {
			mod->U.Proxy = (DWORD)HookFunction((void *)mod->Orig, (void *)mod->Detour);
			if(mod->U.Proxy == 0) {
				FA_Log("FA_Detour_Init 0x%x --> 0x%x Failed!", mod->Orig, mod->Detour);
				return FALSE;
			}
			FA_Log("Detour Org 0x%x --> 0x%x (0x%x) Success!", mod->Orig, mod->Detour, mod->U.Proxy);
		}
		else {
			FA_Log("unhandle mod type %d", mod->Type);
			return FALSE;
		}
	}

	return TRUE;
}

/**
 * [注册MOD信息]
 * @param mod [mod 数组]
 * @param sz  [mod 大小]
 */
void FA_Mod_Register(struct FA_Mod* mod, int sz) {
	memcpy(&__mq.mods[__mq.sz], mod, sz*sizeof(struct FA_Mod));
	__mq.sz += sz;
}

/**
 * [获取函数代理地址]
 * [注意注册的函数不能重复]
 * @param  detour [新地址]
 * @return        [代理地址]
 */
DWORD FA_Detour_GetProxy(DWORD detour) {
	int i,n;
	for(i=0; i<__mq.sz; i++) {
		struct FA_Mod* mod = &__mq.mods[i];
		if(mod->Type == FA_MOD_TYPE_DETOUR && mod->Detour == detour)
			return mod->U.Proxy;
	}

	return 0;
}
