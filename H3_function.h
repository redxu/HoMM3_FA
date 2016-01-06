#ifndef __H3_FUNCTION__H__
#define __H3_FUNCTION__H__

#include "FA_def.h"

/**
 * 将16位位图从内存载入显存
 * @param  pcx [位图名称,如loadbar.pcx]
 * pcx used ecx
 * @return	   [内存地址??]
 */
int FA_FASTCALL H3_BlitPcx16(const char* pcx);

/**
 * 随机函数
 * @param  min [最小可选值]
 * @param  max [最大可选值]
 * @return     [随机值]
 */
int FA_FASTCALL H3_Random(int min, int max);

/**
 * 信息提示框
 * @param  msg      [信息提示]
 * @param  mtype    [类型 1,YES 2,YES_NO]
 * @param  type1    [图标类型 -1不显示 0x14技能图标]
 * @param  stype1   [图标ID]
 * @param  showtime [显示时间 0永久显示]
 * @return          [FA_MESSAGEBOX_YES/NO]
 */
int FA_FASTCALL H3_MessageBox(const char* msg, int mtype, int type1, int stype1, int showtime);

/**
 * [获取当前玩家序号]
 * @return  [0~7]
 */
int H3_GetPlayerIndex(void);

/**
 * [获取当前玩家信息]
 * @param  index [玩家序号-参见H3_GetPlayerIndex]
 * @return       [H3_Player]
 */
struct H3_Player* H3_GetPlayer(int index);

/**
 * [获取英雄信息]
 * @param  heroid [Hero ID]
 * @return        [H3_Hero]
 */
struct H3_Hero* H3_GetHero(int heroid);

/**
 * [英雄学习技能]
 * @param  hero  [英雄地址]
 * @param  skill [技能索引]
 * @param  add   [1添加]
 * @return       [1成功 0失败]
 */
int FA_THISCALL H3_HeroAddSkill(struct H3_Hero* hero, int skill, BYTE add);

/*****	MEMORY	****/
void* __cdecl H3_Malloc(int size);
void __cdecl H3_Free(void* po);

#endif
