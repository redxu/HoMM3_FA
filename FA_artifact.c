/*****************************************************************************
* File Descrip : Artifact Extern
* Create Time  ：20160216
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "FA_mod.h"

//Combo Artifact Count
static int _FA_ComboArtifactCount = H3_MAX_COMBOARTIFACT;
//FA New Artifact Table (Redirect and new)
static struct H3_Artifact __FA_ArtifactTable[FA_MAX_ARTIFACT];
//FA New Artifact Effect Table (Redirect and new)
//0x63E758
static char FA_ArtifactEffect[FA_MAX_ARTIFACT][4];
//FA New Artifact Pickup Msg (Redirect and new)
static char* FA_ArtifactPickupMsg[FA_MAX_ARTIFACT];
//FA New ComboArtifact Table (Redirect and new)
static struct H3_ComboArtifact __FA_ComboArtifactTable[FA_MAX_COMBOARTIFACT] = {
	{0x81,0x80000000,0x0000001F,0x00000000,0x00000000,0x00000000}, // 天使联盟
	{0x82,0x00000000,0x01C00000,0x00000000,0x00000000,0x00000000}, // 鬼王斗篷
	{0x83,0x00000000,0x00000000,0xC0000000,0x00000001,0x00000000}, // 神圣血瓶
	{0x84,0x04104100,0x00000000,0x00000000,0x00000000,0x00000000}, // 诅咒铠甲
	{0x85,0x00000000,0x00000000,0x00000000,0x07C00000,0x00000000}, // 天赐神兵
	{0x86,0x00000000,0x00003FE0,0x00000000,0x00000000,0x00000000}, // 龙王神力
	{0x87,0x41041000,0x00000000,0x00000000,0x00000000,0x00000000}, // 泰坦之箭
	{0x88,0x00000000,0x00000000,0x00000080,0x08000000,0x00000000}, // 海洋之帽
	{0x89,0x00000000,0x70000000,0x00000000,0x00000000,0x00000000}, // 幻影神弓
	{0x8A,0x00000000,0x00000000,0x00000E00,0x00000000,0x00000000}, // 魔力源泉
	{0x8B,0x00000000,0x00000000,0x00007000,0x00000000,0x00000000}, // 法师之戒
	{0x8C,0x00000000,0x00000000,0x00000000,0x0002E000,0x00000000}, // 丰收之角
};
//FA Change Origin Artifact Part to Combo.
struct FA_ComboArtifactPart {
	int sz;
	BYTE index[FA_MAX_ARTIFACT];
	DWORD id[FA_MAX_ARTIFACT];
};

static struct FA_ComboArtifactPart __FA_ComboArtifactPart;

FA_EXPORT struct H3_Artifact* FA_GetArtifact(int artid) {
	struct H3_Artifact *artifact;
	artifact = &__FA_ArtifactTable[artid];
	return artifact;
}

FA_EXPORT int FA_AddArtifact(BYTE id, struct H3_Artifact* artifact) {
	__FA_ArtifactTable[id] = *artifact;
}

/**
 * [Add New Combo Artifact]
 * @param  artid	 [comboa id]
 * @param  count     [artifacts count]
 * @param  va_args   [artifacts id's]
 * @return           [1 success 0 failed]
 */
FA_EXPORT int FA_AddComboArtifact(int artid, int count, ...) {
	va_list l;
	int i, arg;

	//Check artid valid
	if(artid <= 0x8c) {
		return 0;
	}
	for(i = 0; i < _FA_ComboArtifactCount; i++) {
		struct H3_ComboArtifact* art = &__FA_ComboArtifactTable[i];
		if(art->id == artid) {
			//already exists!
			return 0;
		}
	}

	struct H3_ComboArtifact* art = &__FA_ComboArtifactTable[_FA_ComboArtifactCount];
	memset(art, 0, sizeof(*art));
	va_start(l, artid);
	for(i = 0; i < count; i++) {
		arg = va_arg(l, int);
		int zone = arg >> 5;
		int flag = arg & 0x1f;
		flag = 1 << flag;
		art->flags[zone] += flag;

		//record id
		__FA_ComboArtifactPart.id[__FA_ComboArtifactPart.sz] = arg;
		__FA_ComboArtifactPart.index[__FA_ComboArtifactPart.sz] = _FA_ComboArtifactCount;
		__FA_ComboArtifactPart.sz++;
	}
	art->id = artid;
	_FA_ComboArtifactCount++;
	va_end(l);

	return 1;
}

/**
 * [Load Artifact Call]
 * @Patch Artifact Need After this call!
 */
static void* FA_LoadArtifact(void) {
	typedef void* (*F)(void);
	F proxy = (F)FA_Detour_GetProxy((DWORD)FA_LoadArtifact);
	void* rtv = proxy();

	int i;
	for(i = 0; i < __FA_ComboArtifactPart.sz; i++) {
		struct H3_Artifact* artifact = FA_GetArtifact(__FA_ComboArtifactPart.id[i]);
		artifact->combopartindex = __FA_ComboArtifactPart.index[i];
	}

	//Test Code here!
	struct H3_Artifact newart;
	memset(&newart, 0, sizeof(newart));
	newart.name = "Homm3FA Test Combo";
	newart.cost = 10000;
	newart.position = 9;
	newart.type = 0x10;
	newart.description = "Test 100% anti magic!";
	newart.comboindex = 12;
	newart.combopartindex = -1;
	FA_AddArtifact(0x8d, &newart);

	return rtv;
}

//local mods for lod
static struct FA_Mod __mods[] = {
	//Patch H3 Combo Artifact Table
	{FA_MOD_TYPE_BYTE, 0x660B6C, (DWORD)__FA_ComboArtifactTable, 4},
	//Patch H3 Combo Artifact Max Count
	{FA_MOD_TYPE_BYTE, 0x4E33DC+2, FA_MAX_COMBOARTIFACT, 1},
	{FA_MOD_TYPE_BYTE, 0x4E34B1+2, FA_MAX_COMBOARTIFACT, 1},
	{FA_MOD_TYPE_BYTE, 0x4E353D+2, FA_MAX_COMBOARTIFACT, 1},
	{FA_MOD_TYPE_BYTE, 0x4DC3C4+2, FA_MAX_COMBOARTIFACT, 1},
	{FA_MOD_TYPE_BYTE, 0x4DC418+2, FA_MAX_COMBOARTIFACT, 1},
	{FA_MOD_TYPE_BYTE, 0x4DC4A0, FA_MAX_COMBOARTIFACT, 1},
	//Patch H3 Artifact Table
	{FA_MOD_TYPE_BYTE, 0x44CB32, (DWORD)__FA_ArtifactTable, 4},
	{FA_MOD_TYPE_BYTE, 0x44CD1E, (DWORD)__FA_ArtifactTable, 4},
	{FA_MOD_TYPE_BYTE, 0x44CD6C, (DWORD)__FA_ArtifactTable, 4},
	{FA_MOD_TYPE_BYTE, 0x660B68, (DWORD)__FA_ArtifactTable, 4},
	{FA_MOD_TYPE_BYTE, 0x44CCDF, (DWORD)&__FA_ArtifactTable[0].Disable, 4},
	{FA_MOD_TYPE_BYTE, 0x44CCFA, (DWORD)&__FA_ArtifactTable[0].newspell, 4},
	//Patch H3 Artifact Max Count
	{FA_MOD_TYPE_BYTE, 0x44CCA8, (FA_MAX_ARTIFACT+2)*4, 4},
	{FA_MOD_TYPE_BYTE, 0x44CACA, (FA_MAX_ARTIFACT+2)*4, 4},
	//Detour H3 Load Artifact
	{FA_MOD_TYPE_DETOUR, 0x44ca20, (DWORD)FA_LoadArtifact, 0},
};

/**
 * [FA_ARTIFACT_INIT]
 */
void FA_Artifact_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);

	memset(&__FA_ComboArtifactPart, 0, sizeof(struct FA_ComboArtifactPart));
	//抗魔三件套
	FA_AddComboArtifact(0x8d, 3, 0x39, 0x3a, 0x3b);
}
