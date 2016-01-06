#include <windows.h>
#include "FA_def.h"
#include "FA_struct.h"
#include "FA_function.h"


/**
 * [Check Hero Can Learn skill]
 * @param  hero  [hero]
 * @param  skill [skill id]
 * @return       [1 yes 0 no]
 */
int FA_HeroLearnSkillCheck(struct H3_Hero* hero, int skill) {
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
