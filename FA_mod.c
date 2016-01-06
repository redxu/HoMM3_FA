#include "patch.h"
#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "FA_lua.h"
#include "H3_Function.h"
#include "FA_Function.h"
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

//004A7CF0  Visit Witch Hut Call
//004A7DCE      83F8 08       cmp eax,0x8   skillnumlimit
//004E2564  |> \83B9 01010000 08    cmp dword ptr ds:[ecx+0x101],0x8 
/**
 * [Hero Lean Skill From Witch Patch]
 */
static int FA_VisitWitchHut(void) {
	char* msg = (char *)0x697428;
	int skilltype = 0;
	//__asm__　__volatile__ (
	//I don't like AT&T asm -.-
	//skilltype = 3*(skillindex+1)
	__asm (
		"movl %%eax, %0 \n"
		: "=m"(skilltype)
	);
	int rtv = H3_MessageBox(msg, 2, 0x14, skilltype, 0);
	return rtv;
}

/**
  * [Hero Lean Skill From Witch Patch2]
  * [Jude Result,Yes to Lean]
 */
static BYTE FA_VisitWitchHutResult[] = {
	0x3d, 0x05, 0x78, 0x00, 0x00,			//cmp eax,0x7805
	0x75, 0x20								//jnz 4a7e8f
};

//004A4AE6  |.  51            push ecx 开始添加技能,原逻辑先添加再提示
/**
 * FA_HeroLearnSkillFromScholar Param
 * Human Flag
 */
static BYTE FA_HeroLearnSkillFromScholarParam[] = {
	0x8A, 0x45, 0x14,						//mov al,byte ptr ss:[ebp+0x14]
	0x50,									//push eax
};

/**
 * [Hero Learn Skill From Scholar Part1]
 * @return  [1 Learned or Dropped 0 Can't lean]
 */
static FA_STDCALL int FA_HeroLearnSkillFromScholar(BYTE human) {
	//Hero <-- ebx,skill <-- edi
	//ebp+0x14 args humanflag
	struct H3_Hero* hero;
	int skill = 0;
	int rtv;
	__asm (
		"movl %%ebx, %0 \n"
		: "=m"(hero)
	);
	__asm (
		"movl %%edi, %0 \n"
		: "=m"(skill)
	);
	//Get humanflag
	/*
	__asm (
		"pushl %eax \n"
	);
	__asm (
		"movl 0x14(%ebp), %eax \n"
	);
	__asm (
		"movb %%al, %0 \n"
		: "=m"(human)
	);
	__asm (
		"popl %eax \n"
	);
	*/

	rtv = FA_HeroLearnSkillCheck(hero, skill);
	FA_Log("human = %d canlen=%d", human, rtv);
	if(rtv == 0)
		return 0;
	if(human == 1) {
		DWORD *msgaddr = (DWORD *)0x696a68;
		msgaddr = (DWORD *)(*msgaddr + 0x20);
		msgaddr = (DWORD *)(*msgaddr + 0x1cc);
		char* msg = (char *)(*msgaddr);
		rtv = H3_MessageBox(msg, 2, 0x14, 3*(skill+1), 0);
		//must return not zero
		if(rtv == FA_MESSAGEBOX_NO)
			return 1;
	}

	return H3_HeroAddSkill(hero, skill, 1);
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

	typedef void* (FA_FASTCALL* F)(void *);
	F proxy = (F)0x4de980;
	return proxy(ecx);
}

static struct FA_MOD __mods[] = {
	//NewGame->LoadBar
	{0x004ed9ea, (DWORD)FA_LoadBar, FA_MOD_TYPE_CALL, 10},
	//RestartGame->LoadBar
	{0x004ef854, (DWORD)FA_LoadBar, FA_MOD_TYPE_CALL, 10},
	//Visit Witch Hut->Choose Skill
	{0x004a7e63, (DWORD)FA_VisitWitchHut, FA_MOD_TYPE_CALL, 34},
	//Visit Witch Judge Choose Result
	{0x004a7e68, (DWORD)FA_VisitWitchHutResult, FA_MOD_TYPE_WRITE, sizeof(FA_VisitWitchHutResult)},
	//Learn Skill From Scholar Param
	{0x004A4AE6, (DWORD)FA_HeroLearnSkillFromScholarParam, FA_MOD_TYPE_WRITE, sizeof(FA_HeroLearnSkillFromScholarParam)},
	//Learn Skill From Scholar
	{0x004A4AEA, (DWORD)FA_HeroLearnSkillFromScholar, FA_MOD_TYPE_CALL, 5},
	//Learn Skill From Scholar NOP
	{0x004A4AFE, 0, FA_MOD_TYPE_NOP, 59},
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
		else if(mod->Type == FA_MOD_TYPE_WRITE) {
			if(!PatchCode((PVOID)mod->Orig, (PVOID)mod->Detour, mod->Size)) {
				FA_Log("patch 0x%x Failed!", mod->Orig);
				return FALSE;
			}
		}
		else if(mod->Type == FA_MOD_TYPE_NOP) {
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

