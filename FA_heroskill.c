/*****************************************************************************
* File Descrip : Hero skill mod,10 skilltree,Choose Lean Skill etc
* Create Time  ：20160112
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "H3_Function.h"
#include "FA_mod.h"

/**
 * [Check Hero Can Learn skill]
 * @param  hero  [hero]
 * @param  skill [skill id]
 * @return       [1 yes 0 no]
 */
FA_EXPORT int FA_HeroLearnSkillCheck(struct H3_Hero* hero, int skill) {
	int n;
	n = hero->skillcount;

	//check already learned?
	if(hero->skilllv[skill] >= 3) {
		//max level
		return 0;
	}
	else if(hero->skilllv[skill] > 0) {
		return 1;
	}

	//not learned,check level and skillcount
	if(n < 8) {
		return 1;
	}
	else if(n < 9 && hero->level >= 25) {
		return 1;
	}
	else if(n < 10 && hero->level >= 30) {
		return 1;
	}

	return 0;
}

/**
 * [Check Hero can learn new skill]
 * @param  hero [hero]
 * @return      [1 can 0 can't]
 */
FA_EXPORT int FA_HeroLearnNewSkillCheck(struct H3_Hero* hero) {
	int n;
	n = hero->skillcount;
	if(n < 8) {
		return 1;
	}
	else if(n < 9 && hero->level >= 25) {
		return 1;
	}
	else if(n < 10 && hero->level >= 30) {
		return 1;
	}

	return 0;
}


//004A7CF0  Visit Witch Hut Call
//004A7DCE      83F8 08       cmp eax,0x8   skillnumlimit
/**
 * [Hero Lean Skill From Witch Hut]
 */
static void FA_HeroLearnSkillFromWitchHut(void) {
	int rtv;
	char* msg = (char *)0x697428;
	int skilltype = 0;
	//skilltype = 3*(skillindex+1)
	FA_EAX(skilltype);
	rtv = H3_MessageBox(msg, 2, 0x14, skilltype, 0);
	if(rtv == FA_MESSAGEBOX_NO) {
		FA_JMP_D(0x004a7e8f, 4);
	}
	return;
}

/**
 * [Hero Visit WitchHut SkillNum Limit Patch to 10]
 */
static int FA_WitchHut_SkillNum(void) {
	//ebx --> Hero
	struct H3_Hero* hero;
	int rtv;

	FA_EBX(hero);
	rtv = FA_HeroLearnNewSkillCheck(hero);
	//004A7DCE      83F8 08       cmp eax,0x8
	//so make return value <8 or >8
	if(rtv == 0)
		rtv = 9;
	return rtv;
}

//004A4AE6  |.  51           push ecx 开始添加技能,原逻辑先添加再提示
/**
 * [Hero Learn Skill From Scholar]
 * @return  [1 Learned or Dropped 0 Can't lean]
 */
static int FA_HeroLearnSkillFromScholar(void) {
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

//004DAFC8      83BF 01010000 08        cmp dword ptr ds:[edi+0x101],0x8
/**
 * [Hero Lv Up SkillNum Limit Patch to 10]
 */
static void FA_HeroLevelUp_SkillNum(void) {
	struct H3_Hero* hero;
	int rtv;

	FA_EDI(hero);
	rtv = FA_HeroLearnNewSkillCheck(hero);
	//restore eax
	FA_SET_EAX(hero->type);
	if(rtv == 1) {
		FA_JMP_B(0x004DAFDB, 4);
	}
	else {
		FA_JMP_B(0x004DAFD1, 4);
	}
}

//005F0C81  |> \83B9 01010000 08        cmp dword ptr ds:[ecx+0x101],0x8
/**
 * [Hero Learn Skill From University PreCheck]
 */
static void FA_HeroLearnSkillFromUniversity(void) {
	//ecx -->hero
	struct H3_Hero* hero;
	int rtv;

	FA_ECX(hero);
	rtv = FA_HeroLearnNewSkillCheck(hero);
	if(rtv == 1) {
		FA_JMP_D(0x005F0CEA, 4);
	}
}

//005F0F4A  |> \83B8 01010000 08        cmp dword ptr ds:[eax+0x101],0x8
/**
 * [Click skill in University]
 */
static void FA_HeroLearnSkillFromUniversityClick(void) {
	//eax -->hero
	struct H3_Hero* hero;
	int rtv;

	FA_EAX(hero);
	rtv = FA_HeroLearnNewSkillCheck(hero);
	//restore eax
	FA_SET_EAX(hero);
	if(rtv == 1) {
		FA_JMP_D(0x005F0F64, 4);
	}
}

/**
 * [Hero ADPK Limit In HeroInfoDlg]
 */
static BYTE FA_Hero_PSkill_Limit1[] = {
	0x25, 0xff, 0x00, 0x00, 0x00,			//and eax, 0xff
	0xeb, 0x15,								//jmp 04e199b
};

/**
 * [Hero ADPK Limit In SmallPanel]
 */
static BYTE FA_Hero_PSkill_Limit2[] = {
	0x25, 0xff, 0x00, 0x00, 0x00,			//and eax, 0xff
	0xeb, 0x18,								//jmp 04519d5
};

//local mods for heroskill
static struct FA_Mod __mods[] = {
	//Learn Skill From Witch Hut
	{FA_MOD_TYPE_CALL, 0x004a7e63, (DWORD)FA_HeroLearnSkillFromWitchHut, 34},
	//Learn Skill From Scholar
	{FA_MOD_TYPE_CALL, 0x004A4AE6, (DWORD)FA_HeroLearnSkillFromScholar, 9},
	//Learn Skill From Scholar NOP
	{FA_MOD_TYPE_NOP,  0x004A4AFE, 0, 59},
	//Skill Num Limit In Witch Hut
	{FA_MOD_TYPE_CALL, 0x004A7DC8, (DWORD)FA_WitchHut_SkillNum, 6},
	//Skill Num Limit In H3_HeroAddSkill
	{FA_MOD_TYPE_BYTE, 0x004E256A, 0x0a, 1},
	//Skill Num Limit In Hero Level Up
	{FA_MOD_TYPE_CALL, 0x004DAFC8, (DWORD)FA_HeroLevelUp_SkillNum, 7},
	//Learn Skill From University Patch to 10
	{FA_MOD_TYPE_CALL, 0x005F0C81, (DWORD)FA_HeroLearnSkillFromUniversity, 9},
	//Learn Skill From University Patch 2 Skill Num Limit
	{FA_MOD_TYPE_CALL, 0x005F0F4A, (DWORD)FA_HeroLearnSkillFromUniversityClick, 9},
	//Learn Skill From Seer Patch to 10 ... 先知小屋送技能
	{FA_MOD_TYPE_BYTE, 0x0057417F, 0x0a, 1},
	//Hero ADPK(PSKILL) Patch to 255
	//{FA_MOD_TYPE_WRITE, 0x004e197f, (DWORD)FA_Hero_PSkill_Limit1, sizeof(FA_Hero_PSkill_Limit1)},
	//{FA_MOD_TYPE_WRITE, 0x004519b6, (DWORD)FA_Hero_PSkill_Limit2, sizeof(FA_Hero_PSkill_Limit2)},
};


/**
 * [FA_HeroSkill_Init]
 */
void FA_HeroSkill_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);
}
