/*****************************************************************************
* File Descrip : FA Lod manager
* NOT IMPLEMENT!
* Create Time  ：20160114
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "H3_function.h"
#include "FA_debug.h"
#include "FA_mod.h"

static int H3_LOD_NUM = 8;
static const char FA_Lod_Name1[16] = "h3fa_ext.lod";

//Orign LodItemLists Redirect
//Coloum 1:def 2:txt&pic 3:wav
static int __FA_LodItemLists[4][3][FA_MAX_LOD] = {
	{{5,1}, 	{4,0}, 		{1,0}},
	{{7,3,1}, 	{6,2,0}, 	{2,1,0}},
	{{1}, 		{0}, 		{1,0}},
	{{1,3}, 	{0,2}, 		{1,0}}
};
//New LodItemTables..
static struct H3_LodItemTable __FA_LodItemTables[4][3] = {
	{{2,__FA_LodItemLists[0][0]}, {2,__FA_LodItemLists[0][1]}, {2,__FA_LodItemLists[0][2]}},
	{{3,__FA_LodItemLists[1][0]}, {3,__FA_LodItemLists[1][1]}, {3,__FA_LodItemLists[1][2]}},
	{{1,__FA_LodItemLists[2][0]}, {1,__FA_LodItemLists[2][1]}, {2,__FA_LodItemLists[2][2]}},
	//I only checked this, other's copyed from wog.
	{{2,__FA_LodItemLists[3][0]}, {2,__FA_LodItemLists[3][1]}, {2,__FA_LodItemLists[3][2]}}
};

//I don't know how it works...
int FA_LoadCustomLOD(char* lodname) {
	int rtv;
	DWORD addr = 0x0069D8A8;
	addr += ((H3_LOD_NUM - 1) * 0x190);
	//H3_LOD_NUM++;
	rtv = H3_LOD_Init((BYTE *)addr, lodname);
	FA_Log("H3_LOD_Init rtv = %d", rtv);

	char path[FA_MAXPATH];
	int idx;
	memset(path,0,sizeof(path));
	GetModuleFileName(NULL,path,FA_MAXPATH);
	idx = strlen(path)-1;
	while(idx >= 0 && path[idx] != '\\')
	{
		path[idx--] = '\0';
	}
	strcat(path, "Data\\");
	strcat(path, lodname);
	rtv = H3_LOD_LoadHeader((BYTE *)(addr + 4), path, 0);
	FA_Log("H3_LOD_LoadHeader [%s] rtv = %d", path, rtv);

	//add2list

	return 0;
}

/**
 * [Add LOD File To FA_LodItemTables/Custom Lod]
 * @param  itemindex [item index in LodTables]
 * @param  group     [lod group]
 * @param  type      [lod type [0-2]]
 * @return           [1 success 0 exists]
 */
int FA_AddLod2Table(BYTE itemindex, BYTE group, BYTE type) {
	struct H3_LodItemTable* tb = &__FA_LodItemTables[group][type];
	DWORD i,count,*list;
	count = tb->count;
	list = tb->itemlist;
	for(i=0; i<count; i++) {
		if(list[i] == itemindex)
		{
			//exists!
			return 0;
		}
	}
	tb->itemlist[count] = itemindex;
	tb->count++;

	return 1;
}

void FA_ReloadLodItems(void) {
	
}

//local mods for lod
static struct FA_Mod __mods[] = {
	//Patch H3 Init Lod name
	{FA_MOD_TYPE_WRITE, 0x00682F08, (DWORD)FA_Lod_Name1, 16},
	//Redirect LodItemTables
	{FA_MOD_TYPE_BYTE, 0x55A548+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55ABA3+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55AC28+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55AFB1+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55B035+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55B3F8+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55B47D+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55B7BB+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55B840+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x0055bbd1, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55BC53+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55BEE3+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55C1B3+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55C43F+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55CA38+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55CA3F+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55CABB+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55CAC2+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55D17B+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55D212+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55D296+3, (DWORD)&__FA_LodItemTables, 4},
	{FA_MOD_TYPE_BYTE, 0x55CA31+3, (DWORD)&__FA_LodItemTables[0][0].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55CAB4+3, (DWORD)&__FA_LodItemTables[0][0].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55D174+3, (DWORD)&__FA_LodItemTables[0][0].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55AB9C+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55AC21+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55AFAA+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x0055B031, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55B3F1+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55B476+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55B7B4+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55B839+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55BBC7+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55BC4C+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55BEDC+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55C1AC+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55D20B+3, (DWORD)&__FA_LodItemTables[0][1], 4},
	{FA_MOD_TYPE_BYTE, 0x55AB95+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55AC1A+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55AFA3+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55B027+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55B3EA+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55B46F+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55B7AD+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55B832+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55BBC0+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55BC45+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55BED5+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55C1A5+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55D204+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55D28F+3, (DWORD)&__FA_LodItemTables[0][1].itemlist, 4},
	{FA_MOD_TYPE_BYTE, 0x55C438+3, (DWORD)&__FA_LodItemTables[0][2], 4},
};


/**
 * [FA_Lod_Init]
 */
void FA_Lod_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);

	FA_AddLod2Table(4, 3, 0);
	FA_AddLod2Table(4, 3, 1);
}

