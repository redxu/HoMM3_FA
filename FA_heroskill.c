/*****************************************************************************
* File Descrip : Hero skill mod,10 skilltree,Choose Lean Skill etc
* Create Time  ：20160112
* Author	   ：RedXu
*****************************************************************************/

#include "patch.h"
#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "H3_Function.h"
#include "FA_Function.h"
#include "FA_mod.h"

//004A7CF0  Visit Witch Hut Call
//004A7DCE      83F8 08       cmp eax,0x8   skillnumlimit
//004E2564  |> \83B9 01010000 08    cmp dword ptr ds:[ecx+0x101],0x8 
/**
 * [Hero Lean Skill From Witch Patch]
 */
static void FA_STDCALL FA_HeroLearnSkillFromWitchHut(void) {
	int rtv;
	char* msg = (char *)0x697428;
	int skilltype = 0;
	//skilltype = 3*(skillindex+1)
	FA_EAX(skilltype);
	rtv = H3_MessageBox(msg, 2, 0x14, skilltype, 0);
	if(rtv == FA_MESSAGEBOX_NO) {
		FA_JMP(0x004a7e8f, 4);
	}
	return;
}


/**
 * [Hero Learn Skill From Scholar Part1]
 * @return  [1 Learned or Dropped 0 Can't lean]
 */
static int FA_STDCALL FA_HeroLearnSkillFromScholar() {
	//Hero <-- ebx,skill <-- edi
	//caller [ebp+0x14] humanflag
	struct H3_Hero* hero;
	int skill = 0;
	int rtv;
	BYTE human;
	DWORD ebp;
	FA_EBX(hero);
	FA_EDI(skill);
	FA_EBP(ebp);
	ebp = FA_GET_PV(DWORD, ebp);
	human = FA_GET_PV(BYTE, ebp + 0x14);

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

//local mods for heroskill
static struct FA_mod __mods[] = {
	//Learn Skill From Witch Hut
	{FA_MOD_TYPE_CALL, 0x004a7e63, (DWORD)FA_HeroLearnSkillFromWitchHut, 34},
	//Learn Skill From Scholar
	{FA_MOD_TYPE_CALL, 0x004A4AE6, (DWORD)FA_HeroLearnSkillFromScholar, 9},
	//Learn Skill From Scholar NOP
	{FA_MOD_TYPE_NOP,  0x004A4AFE, 0, 59},
};


/**
 * [FA_HeroSkill_Init]
 */
void FA_HeroSkill_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);
}
