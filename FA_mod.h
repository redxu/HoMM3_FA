/*****************************************************************************
* File Descrip : FA Mod Flow
* Create Time  ：20160113
* Author	   ：RedXu
*****************************************************************************/

#ifndef __FA_MOD__H__
#define __FA_MOD__H__

#include <windows.h>
#include "FA_struct.h"

/**
 * 修改HoMM3的流程
 * @return  [TRUE-成功 FALSE-失败]
 */
BOOL FA_Mod_Init(void);

/**
 * [注册MOD信息]
 * @param mod [mod 数组]
 * @param sz  [mod 大小]
 */
void FA_Mod_Register(struct FA_Mod* mod, int sz);


#endif
