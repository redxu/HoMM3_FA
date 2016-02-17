#ifndef __FA_STRUCT__H__
#define __FA_STRUCT__H__

#include "FA_def.h"

/**
 * 英雄结构 0x492
 * refrences http://tieba.baidu.com/p/2689989340
 */
#pragma pack(push, 1)
struct H3_Hero {
	WORD x;
	WORD y;
	WORD z;					// 0 upworld 1 underworld
	BYTE visible;
	DWORD plMapitem;
	BYTE _u1;
	long plotype; 			//??
	DWORD P10Cflag;
	DWORD plSetup;
	WORD mp;				//+18h left spell magic points
	int number;
	DWORD _unk_id;
	BYTE player;			//0~7 ff
	char name[13];			//hero name
	DWORD type;				//+30h 0~17 骑士,牧师,... etc
	BYTE pic;
	WORD movx;				//move to x (0~143)格
	WORD movy;				//move to y (0~143)
	WORD movz;				//move to z (see z)
	BYTE _un2[9];
	BYTE x0;
	BYTE y0;
	BYTE run;
	BYTE direction;			//[0~7] 1:30 3:00 4:30  6:00 7:30 9:00 10:30 12:00
	BYTE flags;
	DWORD maxmovement;		//max movement (div 100 equal steps)
	DWORD curmovement;		//left movement(div 100 equal steps)
	DWORD exp;				//+51h
	WORD level;				//+55h
	DWORD visited[10];
	BYTE _un3[18];
	int creaturetype[7];	//creature type (FFFFFFFF is empty)
	int creaturecount[7];	//off
	BYTE skilllv[28];		//skill level [0~3]
	BYTE skilltree[28];		//skill tree  [0~10 show order]
	DWORD skillcount;		//+101h
	BYTE _un4[17];
	char morale;
	BYTE _un5[3];
	char morale1;
	char luck;
	BYTE _un6[17];
	int artinbody[19][2];	//artifacts in body 4bytes FFFFFFFF
	BYTE freeaddslots;
	char lockedslots[14];	//0未放置 1放置
	int artinpacks[64][2];	//artifacts in package 
	BYTE artinpackscount;	//max 64
	DWORD sex;
	BYTE fly_b;
	DWORD _un7;
	char* mes;
	long _unmes1;
	long _unmes2;
	BYTE spell[70];			//spell 修习地址
	BYTE spellb[70];		//spell 记录地址
	char adpk[4];			//+476h attack defance power knowledge
	BYTE _un8[24];
};

/**
 * 玩家结构 0x168
 */
struct H3_Player {
	char playeridx;			//[0~7]
	char heroescount;
	BYTE _u1[2];
	int curhero;
	int heroes[8];
	int herol;
	int heror;
	BYTE _u2[13];
	char daysleft;			
	char townscount;
	char curtown;
	char towns[48];
	BYTE _u3[4];
	int tophero;
	BYTE _u4[0x54];
	char name[21];			//name.
	BYTE activeflag;		//+E1h 1 active 0 sleep
	BYTE playerflag;		//+E2h 1 player 0 computer
	BYTE _u5[0x45];
	double resimp[8];		//unknow
};

/**
 * H3 Map Item struct 0x26
 */
struct H3_MapItem {
	DWORD setup;
	BYTE land;
	BYTE ltype;
	BYTE river;
	BYTE rtype;
	BYTE road;
	BYTE rdtype;
	WORD _u1;
	BYTE mirror;
	BYTE attrib;
	WORD bits;
	WORD _u2;
	DWORD* draw;
	DWORD* drawend;
	DWORD* drawend2;
	int type;
	WORD stype;
	WORD drawnum;
};

/**
 * H3 core struct 0x4e7cc
 */
struct H3_Core {
	BYTE _u1[0xa4];
	struct H3_Hero heroes[0x9c];
	BYTE _u2[0x1ea];
	WORD dayofweek;
	WORD week;
	WORD month;
};

#pragma pack(pop)

/**
 * H3 Lod Item table struct
 */
struct H3_LodItemTable {
	DWORD count;
	DWORD* itemlist;
};

/**
 * H3 Artifact
 */
struct H3_Artifact {
	char *name;        	// +00h
	DWORD cost;         // +04h
	int position;    	// +08h		//1头部 2披肩 3脖子
	int type;        	// +0Ch		//0x10 0x00 ???
	char *description;  // +10h
	int comboindex;   	// +14h		//>=0组合宝物序号 -1非组合宝物
	int combopartindex;	// +18h		//>=组合部件 -1非组合部件
	BYTE Disable;     	// +1Ch		//0->FALSE
	BYTE newspell;    	// +1Dh		//0->FALSE 1->(泰坦之剑)
	BYTE _u1;           // +1Eh
	BYTE _u2;           // +1Fh
};

/**
 * H3 Combo Artifact 0x18
 */
struct H3_ComboArtifact {
	int id;
	DWORD flags[5];
};

/**
 * H3 Dlg struct/class
 * 0x60 -> 0
 */
struct H3_Dlg {
	DWORD* VTABLE;
	BYTE unknow[0x2c];
	DWORD* itemlist;
	BYTE unknow2[52];
	BYTE* lv[0];
};

/**
 * H3 Dlg Item Command struct
 */
struct H3_DlgItemCmd {
	int type;
	int stype;
	int itemid;
	int flags;
	int xabs;
	int yabs;
	int param;
	int flags2;
};

/**
 * Mod struct
 */
struct FA_Mod {
	DWORD Type;			//Type see FA_def.h
	DWORD Orig;			//Orig address
	DWORD Detour;		//Detour address
	union {
		DWORD Size;		//Mod Size
		DWORD Proxy;	//Detour Proxy
	} U;
};

/**
 * Mod queue struct
 */
struct FA_Mod_Q {
	int sz;
	struct FA_Mod mods[10240];
};

#endif
