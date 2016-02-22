#ifndef __H3_FUNCTION__H__
#define __H3_FUNCTION__H__

#include "FA_def.h"
#include "FA_struct.h"

/**
 * 将16位位图从内存载入显存
 * @param  pcx [位图名称,如loadbar.pcx]
 * @return	   [内存地址??]
 */
//int FA_FASTCALL H3_BlitPcx16(const char* pcx);
#define H3_BlitPcx16(pcx) (((int (FA_FASTCALL *)(const char *))0x55b1e0)(pcx))

/**
 * 随机函数
 * @param  min [最小可选值]
 * @param  max [最大可选值]
 * @return     [随机值]
 */
//int FA_FASTCALL H3_Random(int min, int max);
#define H3_Random(min, max) (((int (FA_FASTCALL *)(int, int))0x50C7C0)(min, max))

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
 * @param  level [添加等级]
 * @return       [1成功 0失败]
 */
int FA_THISCALL H3_HeroAddSkill(struct H3_Hero* hero, int skill, char level);

/**
 * [获取技能名称]
 * @param  skill [技能索引]
 * @return       [名称地址]
 */
char* H3_GetSSkillName(int skill);

/**
 * [获取技能等级名]
 * @param  level [等级[0~3]]
 * @return       [名称地址]
 */
char* H3_GetSSkillLvName(int level);

/**
 * [获取地图大小]
 * @return  [地图大小]
 */
int H3_GetMapSize(void);

/**
 * [获取地图上的物品]
 * @param  x [x 坐标]
 * @param  y [y 坐标]
 * @param  z [z 坐标 0地上 1地下]
 * @return   [item]
 */
struct H3_MapItem* H3_GetMapItem(int x, int y, int z);

/*****	MEMORY	****/
/**
 * H3内存分配
 * @param  size [大小]
 * @return      [内存地址]
 */
//void* FA_CDECL H3_Malloc(int size);
#define H3_Malloc(size) (((void* (FA_CDECL *)(int))0x617492)(size))

/**
 * H3内存释放
 * @param  po [内存地址]
 */
//void FA_CDECL H3_Free(void* po);
#define H3_Free(po) (((void (FA_CDECL *)(void *))0x60B0F0)(po))

/**
 * [H3对话框初始化]
 * @param  dlg   [dlg指针]
 * @param  x     [x坐标]
 * @param  y     [y坐标]
 * @param  dx    [宽]
 * @param  dy    [高]
 * @param  count [0x12]
 * @return       [dlg]
 */
#define H3_DlgCtor(dlg, x, y, dx, dy, count) 							\
 		(((struct H3_Dlg* (FA_THISCALL *)(struct H3_Dlg*, int, int, 	\
 		int, int, int))0x41AFA0)(dlg, x, y, dx, dy, count));

/**
 * [H3对话框执行]
 * @param  dlg   [dlg指针]
 * @param  shown [一般为0]
 */
#define H3_DlgExec(dlg, shown) 											\
 		(((void (FA_THISCALL *)(struct H3_Dlg*, int))0x5ffa20)(dlg, shown));

/**
 * [H3对话框窗口过程]
 * @param  dlg [dlg指针]
 * @param  cmd [cmd指令]
 * @return     [0/1]
 */
#define H3_DlgProc(dlg, cmd) 											\
 		(((DWORD (FA_THISCALL *)(struct H3_Dlg*, struct H3_DlgItemCmd*))\
 		0x41B120)(dlg, cmd));

/**
 * [H3对话框item初始化]
 * @param  dlg    [dlg指针]
 * @param  item   [item指针]
 * @param  zorder [z顺序]
 * @return        []
 */
#define H3_DlgInitItem(dlg, item, zorder) 								\
 		(((int (FA_THISCALL *)(struct H3_Dlg*, BYTE*, int))				\
 		0x5ff270)(dlg, item, zorder));

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
									int itemid, char* pcxname, int flags);

/**
 * [对话框创建并载入DEF文件]
 * @param  addr     [dlgitem 0x48]
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
 * @param  flags    [0x10]
 * @return          [description]
 */
BYTE* FA_THISCALL H3_DlgBuildDefItem(BYTE* addr, int x, int y, int dx, int dy,
								int itemid, char* defname, int defpicid, 
								int p2, int p3, int p4, int flags);

/**
 * [对话框创建并载入DEF文件(原始按钮,如OK,CANCEL)动态?]
 * @param  addr       [dlgitem 0x68]
 * @param  x          [x]
 * @param  y          [y]
 * @param  dx         [width]
 * @param  dy         [height]
 * @param  itemid     [identify]
 * @param  defname    [def filename]
 * @param  defid      [def index]
 * @param  pressdefid [def press index]
 * @param  closedlg   [0 keep 1 close]
 * @param  hotkey     [hotkey]
 * @param  flags      [flags 0x2]
 * @return            [dlgitem]
 */
#define H3_DlgBuildDefItem2(addr, x, y, dx, dy, 					\
							itemid, defname, defid, pressdefid, 	\
							closedlg, hotkey, flags) 				\
		(((BYTE* (FA_THISCALL *)(BYTE*, int, int, int, int, 		\
								int, char*, int, int, 				\
								int, int, int))0x455bd0) 			\
		(addr, x, y, dx, dy, itemid, defname, defid, pressdefid, closedlg, hotkey, flags));

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
								int align, int bkcolor, int flags);

/**
 * [将H3_DlgBuildDefItem的结果保存]
 * @param  list     [description]
 * @param  lastitem [description]
 * @param  count    [description]
 * @param  pitem 	[description]
 * @return          [description]
 */
BYTE* FA_THISCALL H3_DlgAddItem(BYTE* list, BYTE* lastitem, int count, BYTE* pitem);

/**
 * [给DlgItem发送指令]
 * @param  dlg [description]
 * @param  cmd [description]
 */
int FA_THISCALL H3_DlgSendCmd2Item(BYTE* dlg, struct H3_DlgItemCmd* cmd);

/**
 * [检测英雄是否拥有宝物]
 * @param  hero [英雄]
 * @param  artid [宝物id]
 * @return
 */
#define H3_HeroHasArtCheck(hero, artid) (((int (FA_THISCALL *)(struct H3_Hero*, int))0x4d9460)(hero, artid));

/**
 * [初始化LOD文件]
 * @param  lod     [description]
 * @param  lodname [description]
 * @return         [description]
 */
int FA_FASTCALL H3_LOD_Init(BYTE* lod, char* lodname);

int FA_THISCALL H3_LOD_LoadHeader(BYTE* this, const char* fullpath, BYTE readonly);

#define H3_EnableMouse(mouse, enable) (((int (FA_THISCALL *)(BYTE*, BYTE))0x50D7B0)(mouse, enable));

#define H3_SetMouseCursor(mouse, uk, type) (((int (FA_THISCALL *)(BYTE*, int, int))0x50CEA0)(mouse, uk, type));

#endif
