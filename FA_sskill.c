/*****************************************************************************
* File Descrip : Secord Skill Effect Table
* Create Time  ：20160119
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_mod.h"

/**
 * 神秘术恢复表 (1,2,3,4->1,10,20,40)
 */
static int FA_Mysticism_Table[4] = {1, 10, 20, 40};

/**
 * 进攻术加成表 (10/20/30)->(15/30/45)
 */
static float FA_Offence_Table[4] = {0, 0.15, 0.30, 0.45};

/**
 * 抵抗术几率表 (5/10/20)->(7.5/15/30)
 */
static float FA_Resistance_Table[4] = {0, 0.075, 0.15, 0.30};

//local mods for secord skill
static struct FA_Mod __mods[] = {
	//Mysticism_Table
	{FA_MOD_TYPE_WRITE, FA_SSKILL_MYSTICISM_TABLE, (DWORD)&FA_Mysticism_Table, 16},
	//Offence_Table
	{FA_MOD_TYPE_WRITE, FA_SSKILL_OFFENCE_TABLE, (DWORD)&FA_Offence_Table, 16},
	//Resistance_Table
	{FA_MOD_TYPE_WRITE, FA_SSKILL_RESISTANCE_TABLE, (DWORD)&FA_Resistance_Table, 16},
};


/**
 * [FA_HeroSkill_Init]
 */
void FA_SSkill_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);
}