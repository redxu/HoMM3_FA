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
 * @param  mtype    [类型 1,YES 2,YES_NO]
 * @param  type1    [图标类型 -1不显示 0x14技能图标]
 * @param  stype1   [图标ID 默认0]
 * @param  showtime [显示时间 0永久显示]
 * @return          [FA_MESSAGEBOX_YES/NO]
 */
int FA_FASTCALL H3_MessageBox(const char* msg, int mtype, int type1, int stype1, int showtime) {
	int rtv;
	typedef int (FA_FASTCALL *F)(const char* msg, int mtype, int xpos, int ypos, 
							int type1, int stype1, int type2, int stype2,
							int par, int showtime, int type3, int stype3);
	F proxy = (F)0x4f6C00;
	proxy(msg, mtype, -1, -1, type1, stype1, -1, 0, -1, showtime, -1, 0);
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
 * @param  heroid [Hero ID 0~MAXHERO]
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
 * [英雄学习技能]
 * @param  hero  [英雄地址]
 * @param  skill [技能索引]
 * @param  level [添加等级]
 * @return       [1成功 0失败]
 */
int FA_THISCALL H3_HeroAddSkill(struct H3_Hero* hero, int skill, char level) {
	typedef int (FA_THISCALL *F)(struct H3_Hero*, int, char);
	F proxy = (F)0x4e2540;
	return proxy(hero, skill, level);
}

/**
 * [获取技能名称]
 * @param  skill [技能索引]
 * @return       [名称地址]
 */
char* H3_GetSSkillName(int skill) {
	char* sskillname;
	DWORD offset = skill * 16;
	DWORD* addr = (DWORD *)FA_ADDR_SSKILL_NAME_BASE;
	addr = (DWORD *)(*addr + offset);
	sskillname = (char *)(*addr);
	return sskillname;
}

/**
 * [获取技能等级名]
 * @param  level [等级[0~3]]
 * @return       [名称地址]
 */
char* H3_GetSSkillLvName(int level) {
	char* sskilllvname;
	DWORD offset = level * 4;
	DWORD* addr = (DWORD *)(FA_ADDR_SSKILL_LV_NAME_BASE + offset);
	sskilllvname = (char *)(*addr);
	return sskilllvname;
}

/**
 * H3内存分配
 * @param  size [大小]
 * @return      [内存地址]
 */
void* FA_CDECL H3_Malloc(int size) {
	typedef void* (FA_CDECL *F)(int);
	F proxy = (F)0x617492;
	return proxy(size);
}

/**
 * H3内存释放
 * @param  po [内存地址]
 */
void FA_CDECL H3_Free(void* po) {
	typedef void (FA_CDECL *F)(void *);
	F proxy = (F)0x60B0F0;
	proxy(po);
}

/**
 * [对话框创建并载入DEF文件]
 * @param  addr     [description]
 * @param  x        [x]
 * @param  y        [y]
 * @param  dx       [width]
 * @param  dy       [height]
 * @param  itemid   [identify]
 * @param  defname  [def filename]
 * @param  defpicid [def index]
 * @param  p2       [description]
 * @param  p3       [description]
 * @param  p4       [description]
 * @param  flags    [description]
 * @return          [description]
 */
BYTE* FA_THISCALL H3_DlgBuildDefItem(BYTE* addr, int x, int y, int dx, int dy,
								int itemid, char* defname, int defpicid, 
								int p2, int p3, int p4, int flags) {
	typedef BYTE* (FA_THISCALL *F)(BYTE*, int, int, int, int,
								int, char*, int, int, int, int, int);
	F proxy = (F)0x4EA800;
	return proxy(addr, x, y, dx, dy, itemid, defname, defpicid, p2, p3, p4, flags);
}

/**
 * [对话框创建并载入TXT]
 * @param  addr    [description]
 * @param  x       [description]
 * @param  y       [description]
 * @param  dx      [description]
 * @param  dy      [description]
 * @param  text    [description]
 * @param  font    [description]
 * @param  color   [description]
 * @param  itemid  [description]
 * @param  align   [description]
 * @param  bkcolor [description]
 * @param  flags   [description]
 * @return         [description]
 */
BYTE* FA_THISCALL H3_DlgBuildTxtItem(BYTE* addr, int x, int y, int dx, int dy,
								char* text, char* font, int color, int itemid,
								int align, int bkcolor, int flags) {
	typedef BYTE* (FA_THISCALL *F)(BYTE*, int, int, int, int,
								char*, char*, int, int, int, int, int);
	F proxy = (F)0x5BC6A0;
	return proxy(addr, x, y, dx, dy, text, font, color, itemid, align, bkcolor, flags);
}

/**
 * [将H3_DlgBuildDefItem的结果保存]
 * @param  list     [description]
 * @param  lastitem [description]
 * @param  count    [description]
 * @param  pitem 	[description]
 * @return          [description]
 */
BYTE* FA_THISCALL H3_DlgAddItem(BYTE* list, BYTE* lastitem, int count, BYTE* pitem) {
	typedef BYTE* (FA_THISCALL *F)(BYTE*, BYTE*, int, BYTE*);
	F proxy = (F)0x5FE2D0;
	return proxy(list, lastitem, count, pitem);
}

/**
 * [给DlgItem发送指令]
 * @param  dlg [description]
 * @param  cmd [description]
 */
FA_INLINE int FA_THISCALL H3_DlgSendCmd2Item(BYTE* dlg, struct H3_DlgItemCmd* cmd) {
	typedef int (FA_THISCALL *F)(BYTE*, struct H3_DlgItemCmd*);
	F proxy = (F)0x5FF3A0;
	return proxy(dlg, cmd);
}

/**
 * [初始化LOD文件]
 * @param  lod     [description]
 * @param  lodname [description]
 * @return         [description]
 */
int FA_FASTCALL H3_LOD_Init(BYTE* lod, char* lodname) {
	typedef int (FA_FASTCALL *F)(BYTE*, char *);
	F proxy = (F)0x559420;
	return proxy(lod, lodname);
}

int FA_THISCALL H3_LOD_LoadHeader(BYTE* this, const char* fullpath, BYTE readonly) {
	typedef int (FA_THISCALL *F)(BYTE*, const char*, BYTE);
	F proxy = (F)0x4FAF30;
	return proxy(this, fullpath, readonly);
}
