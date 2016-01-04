#include <windows.h>
#include "H3_function.h"



/**
 * 将16位位图从内存载入显存
 * @param  pcx [位图名称,如loadbar.pcx]
 * pcx used ecx
 * @return	   [内存地址??]
 */
int FA_FASTCALL H3_BlitPcx16(const char* pcx) {
	typedef int (FA_FASTCALL *F)(const char *);
	F proxy = (F)0x55b1e0;
	return proxy(pcx);
}

/**
 * 随机函数
 * @param  min [最小可选值]
 * @param  max [最大可选值]
 * @return     [随机值]
 */
int FA_FASTCALL H3_Random(int min, int max) {
	typedef int (FA_FASTCALL *F)(int, int);
	F proxy = (F)0x50C7C0;
	return proxy(min,max);
}

/**
 * 信息提示框
 * @param  msg      [信息提示]
 * @param  mtype    [类型 1,OK]
 * @param  showtime [显示时间 0永久显示]
 * @return          [description]
 */
int FA_FASTCALL H3_MessageBox(const char* msg, int mtype, int showtime) {
	int rtv;
	typedef int (FA_FASTCALL *F)(const char* msg, int mtype, int xpos, int ypos, 
							int type1, int stype1, int type2, int stype2,
							int par, int showtime, int type3, int stype3);
	F proxy = (F)0x4f6C00;
	proxy(msg, mtype, -1, -1, -1, 0, -1, 0, -1, showtime, -1, 0);
	DWORD* ptr = (DWORD*)0x6992D0;
	ptr = (DWORD *)(*ptr+0x38);
	rtv = *ptr;
	return rtv;
}

/**
 * [获取当前玩家序号]
 * @return  [0~7]
 */
int H3_GetPlayerIndex(void) {
	char index;
	char* ptr = (char *)0x69CCF4;
	index = *ptr;
	return (int)index;
}

/**
 * [获取当前玩家信息]
 * @param  index [玩家序号-参见H3_GetPlayerIndex]
 * @return       [H3_Player]
 */
struct H3_Player* H3_GetPlayer(int index) {
	struct H3_Player* _player;
	int offset = index*0x168 + 0x20AD0;
	DWORD* addr = (DWORD *)FA_ADDR_BASE;
	_player = (struct H3_Player *)(*addr + offset);
	return _player;
}

/**
 * [获取英雄信息]
 * @param  heroid [Hero ID]
 * @return        [H3_Hero]
 */
struct H3_Hero* H3_GetHero(int heroid) {
	struct H3_Hero* hero;
	int offset = heroid*0x492 + 0x21620;
	DWORD* addr = (DWORD *)FA_ADDR_BASE;
	hero = (struct H3_Hero *)(*addr + offset);
	return hero;
}

/**
 * H3内存分配
 * @param  size [大小]
 * @return      [内存地址]
 */
void* __cdecl H3_Malloc(int size) {
	void* ptr;
	typedef void* (__cdecl *F)(int);
	F proxy = (F)0x617492;
	ptr = proxy(size);

	return ptr;
}

/**
 * H3内存释放
 * @param  po [内存地址]
 */
void __cdecl H3_Free(void* po) {
	typedef void (__cdecl *F)(void *);
	F proxy = (F)0x60B0F0;
	proxy(po);
}


