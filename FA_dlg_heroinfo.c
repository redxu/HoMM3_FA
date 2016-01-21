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
		id = 0xcb + i;
		H3_DlgBuildDefItem(item, x, y, 0x2c, 38, id, "Secskill38.def", 0, 0, 0, 0, 0x10);

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
		id = 0xd5 + i;
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
		id = 0xdf + i;
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

	FA_JMP_D(0x004DFF65, 4);
}

/**
 * [HeroInfo Dlg Update 10 Skills]
 */
static void FA_DlgHeroInfoUpdateSkill10(void) {
	DWORD _dlg_;
	int i,j;
	int skillindex;
	struct H3_Hero* hero;
	struct H3_DlgItemCmd cmd;
	char* sskillname;
	char* sskilllvname;
	int sskilllv;

	//save in ebx?
	FA_EBX(_dlg_);
	//004E2198  |> /A1 708B6900   /mov eax,dword ptr ds:[0x698B70]
	hero = FA_GET_PV(DWORD, 0x698B70);
	for(i=0; i<hero->skillcount; i++) {
		//查找第i个技能
		skillindex = -1;
		for(j=0; j<28; j++) {
			if(hero->skilltree[j] == i+1) {
				skillindex = j;
				break;
			}
		}
		if(skillindex == -1) {
			FA_Log("错误!未找到第%d个技能", i+1);
			continue;
		}
		sskilllv = hero->skilllv[skillindex];
		//技能图标
		memset(&cmd, 0, sizeof(cmd));
		cmd.type = 0x200;
		cmd.stype = 4;
		cmd.itemid = 0xcb + i;
		cmd.param = 2 + skillindex*3 + sskilllv;	//def index.
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		//技能名称
		sskillname = H3_GetSSkillName(skillindex);
		cmd.stype = 3;
		cmd.itemid = 0xd5 + i;
		cmd.param = sskillname;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		//技能等级
		sskilllvname = H3_GetSSkillLvName(sskilllv);
		cmd.itemid = 0xdf + i;
		cmd.param = sskilllvname;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		//
		cmd.stype = 5;
		cmd.itemid = 0xcb + i;
		cmd.param = 4;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		cmd.stype = 5;
		cmd.itemid = 0xd5 + i;
		cmd.param = 4;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		cmd.stype = 5;
		cmd.itemid = 0xdf + i;
		cmd.param = 4;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
	}
	for(i=hero->skillcount; i<8; i++) {
		cmd.type = 0x200;
		cmd.stype = 6;
		cmd.itemid = 0xcb + i;
		cmd.param = 4;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		cmd.type = 0x200;
		cmd.stype = 6;
		cmd.itemid = 0xd5 + i;
		cmd.param = 4;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		cmd.type = 0x200;
		cmd.stype = 6;
		cmd.itemid = 0xdf + i;
		cmd.param = 4;
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);	
	}
	//Lock SSkill 9,10
	if(hero->skillcount <=8 && hero->level < 25) {
		memset(&cmd, 0, sizeof(cmd));
		cmd.type = 0x200;
		cmd.stype = 4;
		cmd.itemid = 0xcb + 8;
		cmd.param = 2;			//def index.
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		cmd.stype = 3;
		cmd.itemid = 0xd5 + 8;
		cmd.param = "Lv25 Unlock";			
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
	}
	if(hero->skillcount <=9 && hero->level < 30) {
		memset(&cmd, 0, sizeof(cmd));
		cmd.type = 0x200;
		cmd.stype = 4;
		cmd.itemid = 0xcb + 9;
		cmd.param = 2;			//def index.
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
		cmd.stype = 3;
		cmd.itemid = 0xd5 + 9;
		cmd.param = "Lv30 Unlock";			
		H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
	}
}

/**
 * [HeroInfo Dlg MouseClick 10 Skills]
 */
static void FA_DlgHeroInfoMouseClickSkill10(void) {
	DWORD itemid;
	int skillindex;

	FA_EAX(itemid);
	if(itemid < 0xcb || itemid > 0xE8) {
		FA_JMP_D(0x4DD5B9,4);
	}
	else if(itemid >= 0xcb && itemid < 0xd5) {
		skillindex = itemid - 0xcb;
	}
	else if(itemid >= 0xd5 && itemid < 0xdf) {
		skillindex = itemid - 0xd5;
	}
	else
	{
		skillindex = itemid - 0xdf;
	}
	FA_SET_EAX(skillindex);
}

/**
 * [HeroInfo Dlg MouseMove 10 Skills]
 */
static void FA_DlgHeroInfoMouseMoveSkill10(void) {
	DWORD itemid;
	int skillindex;

	FA_EAX(itemid);
	if(itemid < 0xcb || itemid > 0xE8) {
		FA_JMP_D(0x4DBEB2,4);
	}
	else if(itemid >= 0xcb && itemid < 0xd5) {
		skillindex = itemid - 0xcb;
	}
	else if(itemid >= 0xd5 && itemid < 0xdf) {
		skillindex = itemid - 0xd5;
	}
	else
	{
		skillindex = itemid - 0xdf;
	}
	FA_SET_ECX(skillindex);
}

//HeroInfo Dlg Exp and Spell def
static const char* PSKIL38 = "PSKIL38.def";


//local mods for dlg_heroinfo
static struct FA_Mod __mods[] = {
	//HeroInfo Dlg Show 10 Skills
	{FA_MOD_TYPE_CALL, 0x4DF7C9, (DWORD)FA_DlgHeroInfoDrawSkill10, 0x4f},
	//HeroInfo Dlg Show Exp and Spell
	{FA_MOD_TYPE_WRITE, 0x4e0dd3+1, (DWORD)&PSKIL38, 4},
	{FA_MOD_TYPE_BYTE, 0x4e0dda+1, 0x26, 1},
	{FA_MOD_TYPE_BYTE, 0x4e0ddf, 0xe2, 1},
	{FA_MOD_TYPE_WRITE, 0x4e0e1d+1, (DWORD)&PSKIL38, 4},
	{FA_MOD_TYPE_BYTE, 0x4e0e24+1, 0x26, 1},
	{FA_MOD_TYPE_BYTE, 0x4e0e28+1, 0xe2, 1},
	//text 0x4ded40 6c 6d 0x4e1218.id 70 71 
	{FA_MOD_TYPE_BYTE, 0x4ded67+1, 0xe3, 1},
	{FA_MOD_TYPE_BYTE, 0x4dedb7+1, 0xe3, 1},
	{FA_MOD_TYPE_BYTE, 0x4e123f+1, 0xf6, 1},
	{FA_MOD_TYPE_BYTE, 0x4e1289+1, 0xf6, 1},
	//HeroInfo Dlg_update.
	{FA_MOD_TYPE_CALL, 0x4E2190, (DWORD)FA_DlgHeroInfoUpdateSkill10, 0x1ac},
	//HeroInfo Dlg mouse click
	{FA_MOD_TYPE_CALL, 0x4DE779, (DWORD)FA_DlgHeroInfoMouseClickSkill10, 0x33},
	//HeroInfo Dlg mouse move
	{FA_MOD_TYPE_CALL, 0x4DBE2B, (DWORD)FA_DlgHeroInfoMouseMoveSkill10, 0x2b},
	//HeroInfo Dlg bg -> heroscr5.pcx
	{FA_MOD_TYPE_WRITE, 0x00679DB8, (DWORD)"heroscr5.pcx", 12},
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
