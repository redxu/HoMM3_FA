#include <windows.h>
#include "H3_function.h"



/**
 * 将16位位图从内存载入显存
 * @param  pcx [位图名称,如loadbar.pcx]
 * @return	   [内存地址??]
 */
/*
int FA_FASTCALL H3_BlitPcx16(const char* pcx) {
	typedef int (FA_FASTCALL *F)(const char *);
	F proxy = (F)0x55b1e0;
	return proxy(pcx);
}
*/

/**
 * 随机函数
 * @param  min [最小可选值]
 * @param  max [最大可选值]
 * @return     [随机值]
 */
/*
int FA_FASTCALL H3_Random(int min, int max) {
	typedef int (FA_FASTCALL *F)(int, int);
	F proxy = (F)0x50C7C0;
	return proxy(min,max);
}
*/

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
 * [获取地图大小]
 * @return  [地图大小]
 */
int H3_GetMapSize(void) {
	int sz;
	DWORD offset = 0x1FC44;
	DWORD* addr = (DWORD *)FA_ADDR_BASE;
	sz = FA_GET_PV(int, *addr + offset);
	return sz;
}

/**
 * [获取地图上的物品]
 * @param  x [x 坐标]
 * @param  y [y 坐标]
 * @param  z [z 坐标 0地上 1地下]
 * @return   [item]
 */
struct H3_MapItem* H3_GetMapItem(int x, int y, int z) {
	struct H3_MapItem *items, *item;
	int msz;
	msz = H3_GetMapSize();
	DWORD offset = 0x1FC40;
	DWORD* addr = (DWORD *)FA_ADDR_BASE;
	items = FA_GET_PV(struct H3_MapItem *, *addr + offset);
	item = items + (x + (y + z * msz) * msz);
	return item;
}

/**
 * H3内存分配
 * @param  size [大小]
 * @return      [内存地址]
 */
/*
void* FA_CDECL H3_Malloc(int size) {
	typedef void* (FA_CDECL *F)(int);
	F proxy = (F)0x617492;
	return proxy(size);
}
*/

/**
 * H3内存释放
 * @param  po [内存地址]
 */
/*
void FA_CDECL H3_Free(void* po) {
	typedef void (FA_CDECL *F)(void *);
	F proxy = (F)0x60B0F0;
	proxy(po);
}
*/

static DWORD H3_Dlg_VTable[15] = {
	0x5D2900, // Dctor
	0x5FF0A0, // Activate
	0x5FF220, // Disactivate
	0x405610,  
	0x49A230,
	0x5FF5E0,
	0x5FFA20,
	0x5FFB30,
	0x5FFBB0,
	/*0x05D7F30,*/ 0x41B120, // action ???
	0x5FFCA0,
	0x5FFD50,
	0x5FFE90,
	0x4842C0,
	0x41B0F0
};

int FA_THISCALL H3_DlgEventLoop(BYTE* _dlg_, struct H3_DlgItemCmd* cmd) {
	typedef int (FA_THISCALL *F)(BYTE*, struct H3_DlgItemCmd* );
	F proxy = (F)0x41B120;	//Dlg_EventLoop
	return proxy(_dlg_, cmd);
}

BYTE* H3_DlgCtor(int x, int y, int dx, int dy, int itemcount) {
	//Malloc and Call Ctor
	BYTE* _dlg_ = (BYTE *)H3_Malloc(0x68);
	typedef BYTE* (FA_THISCALL *F)(BYTE*, int, int, int, int, int);
	F proxy = (F)0x41AFA0;
	proxy(_dlg_, x, y, dx, dy, 18);
	//Set VTABLE
	FA_SET_PV(BYTE, _dlg_ + 0x60, 0);
	FA_SET_PV(DWORD, _dlg_, FA_DLG_SYSOPTIONS_VTABLE);
	//Set ItemList
	BYTE* _items_ = (BYTE *)H3_Malloc(itemcount);
	FA_SET_PV(DWORD, _dlg_ + 0x34, _items_);
	FA_SET_PV(DWORD, _dlg_ + 0x38, _items_);
	FA_SET_PV(DWORD, _dlg_ + 0x3c, &_items_[itemcount]);
	//what's 0x64?
	FA_SET_PV(DWORD, _dlg_ + 0x64, 1);
	return _dlg_;
}

int FA_THISCALL H3_DlgExec(BYTE* _dlg_, int unknow) {
	typedef int (FA_THISCALL *F)(BYTE*, int);
	F proxy = (F)0x5ffa20;
	return proxy(_dlg_, unknow);
}

int FA_THISCALL H3_DlgActive(BYTE* _dlg_, char active) {
	typedef int (FA_THISCALL *F)(BYTE*, int);
	F proxy = (F)0x5ffbb0;
	return proxy(_dlg_, active);
}

int FA_THISCALL H3_DlgShow(BYTE* _dlg_, int zorder, int draw) {
	typedef int (FA_THISCALL *F)(BYTE*, int, int);
	F proxy = (F)0x5FF0A0;
	return proxy(_dlg_, zorder, draw);
}

/**
 * [对话框创建并载入pcx图片]
 * @param  addr    [description]
 * @param  x       [x]
 * @param  y       [y]
 * @param  dx      [width]
 * @param  dy      [height]
 * @param  itemid  [identify]
 * @param  pcxname [pcx name]
 * @param  flags   [description]
 * @return         [description]
 */
BYTE* FA_THISCALL H3_DlgBuildPcxItem(BYTE* addr, int x, int y, int dx, int dy,
									int itemid, char* pcxname, int flags) {
	typedef BYTE* (FA_THISCALL *F)(BYTE*, int, int, int, int,
									int, char*, int);
	F proxy = (F)0x44FFA0;
	return proxy(addr, x, y, dx, dy, itemid, pcxname, flags);
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
