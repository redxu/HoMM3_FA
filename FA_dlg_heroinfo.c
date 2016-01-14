/*****************************************************************************
* File Descrip : HeroInfo Dlg Mod
* Create Time  ：20160113
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "H3_Function.h"
#include "FA_mod.h"

//004DF7C9  |.  6A 48         push 0x48  -->Dlg_HeroInfo
//---->def---->hint1---->hint2
/**
 * [HeroInfo Dlg Show 10 Skills]
 */
static void FA_DlgHeroInfoDrawSkill10(void) {
	BYTE* item;
	DWORD callee_ebp, caller_ebp, caller_esi;
	int i;
	int x, y, id;

	//Draw hero skill panel
	for(i=0; i<10; i++) {
		//def.
		item = (BYTE *)H3_Malloc(0x48);
		if(item == NULL) {
			//TODO: need some dirty work! bur realy happend?
			FA_Log("H3_Malloc Failed!!!");
			return;
		}
		x = (i % 2 == 0 ? 0x12 : 0xa1);
		y = 267 + (i / 2) * 40;
		//Orign ID 0x4f -> Redirect To 0x4b
		id = 0x4b + i;
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

		//hint1
		item = (BYTE *)H3_Malloc(0x50);
		if(item == NULL) {
			//TODO: need some dirty work! bur realy happend?
			FA_Log("H3_Malloc Failed!!!");
			return;
		}
		x = (i % 2 == 0 ? 0x44 : 0xD3);
		y = 287 + (i / 2) * 39;
		//Orign ID 0x57 -> Redirect To 0x55
		id = 0x55 + i;
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

		//hint2
		item = (BYTE *)H3_Malloc(0x50);
		if(item == NULL) {
			//TODO: need some dirty work! bur realy happend?
			FA_Log("H3_Malloc Failed!!!");
			return;
		}
		x = (i % 2 == 0 ? 0x44 : 0xD3);
		y = 267 + (i / 2) * 40;
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

	FA_JMP(0x004DFF65, 4);
}


//local mods for dlg_heroinfo
static struct FA_mod __mods[] = {
	//HeroInfo Dlg Show 10 Skills
	{FA_MOD_TYPE_CALL, 0x004DF7C9, (DWORD)FA_DlgHeroInfoDrawSkill10, 0x4f},
	//HeroInfo Dlg_update.
	{FA_MOD_TYPE_BYTE, 0x4e2191, 0x4b, 1},
	{FA_MOD_TYPE_BYTE, 0x4e219f, 0xb5, 1},
	{FA_MOD_TYPE_BYTE, 0x4e21b0, 0xb6, 1},
	{FA_MOD_TYPE_BYTE, 0x4e221e, 0x0a, 1},
	{FA_MOD_TYPE_BYTE, 0x4e2286, 0x14, 1},
	{FA_MOD_TYPE_BYTE, 0x4e22b8, 0x0a, 1},
	{FA_MOD_TYPE_BYTE, 0x4e2309, 0x0a, 1},
	{FA_MOD_TYPE_BYTE, 0x4e231d, 0x14, 1},
	{FA_MOD_TYPE_BYTE, 0x4e2332, 0xB5, 1},
	{FA_MOD_TYPE_BYTE, 0x4e2335, 0x0a, 1},
	//HeroInfo Dlg mouse click
	{FA_MOD_TYPE_BYTE, 0x4de77b, 0x4b, 1},
	{FA_MOD_TYPE_BYTE, 0x4de780, 0x54, 1},
	{FA_MOD_TYPE_BYTE, 0x4de785, 0xb5, 1},
	{FA_MOD_TYPE_BYTE, 0x4de78a, 0x55, 1},
	{FA_MOD_TYPE_BYTE, 0x4de794, 0xab, 1},
	{FA_MOD_TYPE_BYTE, 0x4de7a2, 0x68, 1},
	//HeroInfo Dlg mouse move
	{FA_MOD_TYPE_BYTE, 0x4dbe2d, 0x4b, 1},
	{FA_MOD_TYPE_BYTE, 0x4dbe32, 0x54, 1},
	{FA_MOD_TYPE_BYTE, 0x4dbe37, 0xb5, 1},
	{FA_MOD_TYPE_BYTE, 0x4dbe3c, 0x55, 1},
	{FA_MOD_TYPE_BYTE, 0x4dbe46, 0xab, 1},
	{FA_MOD_TYPE_BYTE, 0x4dbe50, 0x68, 1},
	//TEST mouse click hero head
	//{0x004e1afa, (DWORD)TestClick, FA_MOD_TYPE_CALL, 5},
};

/**
 * [FA_DlgHeroInfo_Init]
 */
void FA_DlgHeroInfo_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);
}
