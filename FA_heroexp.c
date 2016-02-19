/*****************************************************************************
* File Descrip : Hero Level up Exp Mod (After Lv12)
* Create Time  ：20160218
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "FA_mod.h"

/**
 * 英雄12级以后升级的经验系数(1.2->1.1)
 */
static double FA_HeroLv12ExpRatio = 1.1;


//00469FBF  |.  81C3 F4010000 add ebx,0x1F4
/**
 * [杀死英雄获取的经验值]
 * 由固定500改为根据英雄等级来判断
 */
static void FA_KillHeroExp(void) {
	//ebx->Monster Exp ecx->Defeated Hero eax->combatMgr
	DWORD mexp, hexp;
	DWORD combatmgr;
	struct H3_Hero* hero;
	FA_EBX(mexp);
	FA_ECX(hero);
	FA_EAX(combatmgr);

	//等级小于5,固定300Exp,每增长5级,经验递增250
	if(hero->level <= 5) {
		hexp = 300;
	}
	else {
		hexp = 500 + ((hero->level - 6) / 5) * 250;
	}
	mexp += hexp;
	FA_SET_EBX(mexp);
	//restore eax
	FA_SET_EAX(combatmgr);
}



//local mods for heroexp
static struct FA_Mod __mods[] = {
	//Mod Hero Lv12 ExpRatio
	{FA_MOD_TYPE_WRITE, FA_ADDR_HERO_LVUPEXP12, (DWORD)&FA_HeroLv12ExpRatio, 8},
	//Mod Battle Kill Hero Exp
	{FA_MOD_TYPE_CALL, 0x00469FBF, (DWORD)FA_KillHeroExp, 6},
};

/**
 * [FA_HeroExp_Init]
 */
void FA_HeroExp_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);
}
