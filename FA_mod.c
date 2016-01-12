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
 * (Etc,Human flag can get from curplayer.)
 */
static BYTE FA_HeroLearnSkillFromScholarParam[] = {
	0x8A, 0x45, 0x14,						//mov al,byte ptr ss:[ebp+0x14]
	0x50,									//push eax
};

/**
 * [Hero Learn Skill From Scholar Part1]
 * @return  [1 Learned or Dropped 0 Can't lean]
 */
static int FA_STDCALL FA_HeroLearnSkillFromScholar(BYTE human) {
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

//004DF7C9  |.  6A 48         push 0x48  -->Dlg_HeroInfo
static void FA_STDCALL FA_HeroInfoDlgDrawSkillDef10(void) {
	BYTE* item;
	DWORD callee_ebp, caller_ebp, caller_esi;
	int i;

	//Draw hero skill panel
	for(i=0; i<10; i++) {
		item = (BYTE *)H3_Malloc(0x48);
		if(item == NULL) {
			//TODO: need some dirty work! bur realy happend?
			FA_Log("H3_Malloc Failed!!!");
			return;
		}
		int x = (i % 2 == 0 ? 0x12 : 0xa1);
		//int y = 0x114 + (i / 2) * 40;
		int y = 267 + (i / 2) * 40;
		//int id = 0x4f + i;	//4f~58
		//Orign ID 0x4f -> Redirect To 0x4b
		int id = 0x4b + i;
		H3_DlgBuildDefItem(item, x, y, 0x2c, 38, id, "Secskill.def", 0, 0, 0, 0, 0x10);

		//black magic code
		//Get Caller local vars.
		FA_EBP(callee_ebp);
		FA_ESI(caller_esi);		//caller esi eq callee esi.
		caller_ebp = FA_GET_PV(DWORD, callee_ebp);
		//885D FC       mov byte ptr ss:[ebp-0x4],bl  //what's bl? looks like always 0?
		//8945 E0       mov [ebp-0x20],eax			//eax == item;
		FA_SET_PV(DWORD, caller_ebp - 0x20, item);

		H3_DlgAddItem((BYTE *)caller_esi, (BYTE *)(FA_GET_PV(DWORD, (caller_esi + 8))), 1, (BYTE *)(caller_ebp - 0x20));
	}

	//black magic again
	DWORD jmpaddr = 0x004DFA4D;
	__asm __volatile__ (
		"movl %0, %%edx \n"
		: "=m"(jmpaddr)
	);
	__asm __volatile__ (
		"leave \n"
	);
	__asm __volatile__ (
		"jmp *%edx \n"
	);
}

//draw hint info
static void FA_STDCALL FA_HeroInfoDlgDrawSkillDesc10(void) {
	BYTE* item;
	DWORD callee_ebp, caller_ebp, caller_esi;
	int i;

	//Draw hero skill panel
	for(i=0; i<10; i++) {
		//
		item = (BYTE *)H3_Malloc(0x50);
		if(item == NULL) {
			//TODO: need some dirty work! bur realy happend?
			FA_Log("H3_Malloc Failed!!!");
			return;
		}
		int x = (i % 2 == 0 ? 0x44 : 0xD3);
		int y = 287 + (i / 2) * 39;
		//int id = 0x57 + i;	//4f~58
		int id = 0x55 + i;
		H3_DlgBuildTxtItem(item, x, y, 0x5a, 0x12, NULL, "smalfont.fnt", 1, id, 0, 0, 0x8);

		//black magic code
		//Get Caller local vars.
		FA_EBP(callee_ebp);
		FA_ESI(caller_esi);		//caller esi eq callee esi.
		caller_ebp = FA_GET_PV(DWORD, callee_ebp);
		//885D FC       mov byte ptr ss:[ebp-0x4],bl  //what's bl? looks like always 0?
		//8945 E0       mov [ebp-0x20],eax			//eax == item;
		FA_SET_PV(DWORD, caller_ebp - 0x20, item);

		H3_DlgAddItem((BYTE *)caller_esi, (BYTE *)(FA_GET_PV(DWORD, (caller_esi + 8))), 1, (BYTE *)(caller_ebp - 0x20));


		item = (BYTE *)H3_Malloc(0x50);
		if(item == NULL) {
			//TODO: need some dirty work! bur realy happend?
			FA_Log("H3_Malloc Failed!!!");
			return;
		}
		x = (i % 2 == 0 ? 0x44 : 0xD3);
		y = 267 + (i / 2) * 40;
		//id = 0x5f + i;	//5f~69
		id = 0x5f + i;
		H3_DlgBuildTxtItem(item, x, y, 0x5a, 0x12, NULL, "smalfont.fnt", 1, id, 0, 0, 0x8);

		//black magic code
		//Get Caller local vars.
		FA_EBP(callee_ebp);
		FA_ESI(caller_esi);		//caller esi eq callee esi.
		caller_ebp = FA_GET_PV(DWORD, callee_ebp);
		//885D FC       mov byte ptr ss:[ebp-0x4],bl  //what's bl? looks like always 0?
		//8945 E0       mov [ebp-0x20],eax			//eax == item;
		FA_SET_PV(DWORD, caller_ebp - 0x20, item);

		H3_DlgAddItem((BYTE *)caller_esi, (BYTE *)(FA_GET_PV(DWORD, (caller_esi + 8))), 1, (BYTE *)(caller_ebp - 0x20));
	}

	//black magic again
	DWORD jmpaddr = 0x004DFF65;
	__asm __volatile__ (
		"movl %0, %%edx \n"
		: "=m"(jmpaddr)
	);
	__asm __volatile__ (
		"leave \n"
	);
	__asm __volatile__ (
		"jmp *%edx \n"
	);
}

//004E2190  |.  BF 4F000000   mov edi,0x4F
//004E2284  |.  8D46 10       |lea eax,dword ptr ds:[esi+0x10] ->path1.
//004E22B6  |.  8D46 08       |lea eax,dword ptr ds:[esi+0x8]
//004E2333      83F9 08       cmp ecx,0x8
//update heroinfo.
static void FA_STDCALL FA_HeroInfoDlgUpdate(void) {

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
	//test redraw hero screen
	{0x004DF7C9, (DWORD)FA_HeroInfoDlgDrawSkillDef10, FA_MOD_TYPE_CALL, 0x4f},
	{0x004DFA4D, (DWORD)FA_HeroInfoDlgDrawSkillDesc10, FA_MOD_TYPE_CALL, 0x50},
	//dlg_hero_update.
	{0x4e2191, 0x4b, FA_MOD_TYPE_BYTE, 1},
	{0x4e219f, 0xb5, FA_MOD_TYPE_BYTE, 1},
	{0x4e21b0, 0xb6, FA_MOD_TYPE_BYTE, 1},
	{0x4e221e, 0x0a, FA_MOD_TYPE_BYTE, 1},
	{0x4e2286, 0x14, FA_MOD_TYPE_BYTE, 1},
	{0x4e22b8, 0x0a, FA_MOD_TYPE_BYTE, 1},
	{0x4e2309, 0x0a, FA_MOD_TYPE_BYTE, 1},
	{0x4e231d, 0x14, FA_MOD_TYPE_BYTE, 1},
	{0x4e2332, 0xB5, FA_MOD_TYPE_BYTE, 1},
	{0x4e2335, 0x0a, FA_MOD_TYPE_BYTE, 1},
	//mouse click? rejudge
	{0x4de77b, 0x4b, FA_MOD_TYPE_BYTE, 1},
	{0x4de780, 0x54, FA_MOD_TYPE_BYTE, 1},
	{0x4de785, 0xb5, FA_MOD_TYPE_BYTE, 1},
	{0x4de78a, 0x55, FA_MOD_TYPE_BYTE, 1},
	{0x4de794, 0xab, FA_MOD_TYPE_BYTE, 1},
	{0x4de7a2, 0x68, FA_MOD_TYPE_BYTE, 1},
	//mouse move?
	{0x4dbe2d, 0x4b, FA_MOD_TYPE_BYTE, 1},
	{0x4dbe32, 0x54, FA_MOD_TYPE_BYTE, 1},
	{0x4dbe37, 0xb5, FA_MOD_TYPE_BYTE, 1},
	{0x4dbe3c, 0x55, FA_MOD_TYPE_BYTE, 1},
	{0x4dbe46, 0xab, FA_MOD_TYPE_BYTE, 1},
	{0x4dbe50, 0x68, FA_MOD_TYPE_BYTE, 1},
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
		else if(mod->Type == FA_MOD_TYPE_BYTE) {
			if(!PatchCode((PVOID)mod->Orig, (BYTE *)&mod->Detour, mod->Size)) {
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

