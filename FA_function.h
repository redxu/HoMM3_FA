/*****************************************************************************
* File Descrip : FA New/Reimplement Functions
* Create Time  ：20160113
* Author	   ：RedXu
*****************************************************************************/

#ifndef __FA_FUNCTION__H__
#define __FA_FUNCTION__H__

/**
 * [Check Hero Can Learn skill]
 * @param  hero  [hero]
 * @param  skill [skill id]
 * @return       [1 yes 0 no]
 */
int FA_HeroLearnSkillCheck(struct H3_Hero* hero, int skill);

/**
 * [Check Hero can learn new skill]
 * @param  hero [hero]
 * @return      [1 can 0 can't]
 */
int FA_HeroLearnNewSkillCheck(struct H3_Hero* hero);

#endif
