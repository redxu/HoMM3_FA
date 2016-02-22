/*****************************************************************************
* File Descrip : FA Dlg Function Wrap
* Create Time  ：20160221
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "H3_function.h"

/**
 * [FA对话框执行]
 * @param  dlg   [dlg指针]
 * @param  shown [一般为0]
 */
static void FA_THISCALL FA_DlgExec(struct H3_Dlg* dlg, int shown) {
	BYTE* mouse = (BYTE *)FA_GET_PV(DWORD, FA_ADDR_MOUSEMGR);
	H3_EnableMouse(mouse, 1);
	H3_SetMouseCursor(mouse, 0, 0);

	H3_DlgExec(dlg, shown);
}

/**
 * [Create Dlg Default VTable]
 * @return  [VTable]
 */
FA_EXPORT
struct H3_Dlg_VTable* FA_DlgVTableCreate(void) {
	struct H3_Dlg_VTable* vt = (struct H3_Dlg_VTable*)H3_Malloc(sizeof(*vt));
	struct H3_Dlg_VTable dvtable = {
		0x5D2900, 			// Dctor
		0x5FF0A0, 			// show
		0x5FF220, 			// hide
		0x405610,  
		0x49A230,
		0x5FF5E0,
		FA_DlgExec,			// exec
		0x5FFB30,
		0x5FFBB0,
		0x41B120, 			// virtual dlgproc
		0x5FFCA0,
		0x5FFD50,
		0x5FFE90,
		0x4842C0,
		0x41B0F0
	};
	memcpy(vt, &dvtable, sizeof(*vt));
	return vt;
}

/**
 * [Create Dlg]
 * @param  x     [x]
 * @param  y     [y]
 * @param  dx    [width]
 * @param  dy    [height]
 * @param  count [item count]
 * @param  vt    [virtual table]
 * @return       [dlg]
 */
FA_EXPORT 
struct H3_Dlg* FA_DlgCreate(int x, int y, int dx, int dy, int count, struct H3_Dlg_VTable* vt) {
	struct H3_Dlg* _dlg = (struct H3_Dlg*)H3_Malloc(0x68);
	H3_DlgCtor(_dlg, x, y, dx, dy, 0x12);
	//Set VTable
	_dlg->vtable = vt;
	//Set itemlist
	BYTE* _items = (BYTE*)H3_Malloc(count);
	_dlg->itemlist.first = _items;
	_dlg->itemlist.last = _items;
	_dlg->itemlist.end = &_items[count];
	//Set Flag
	_dlg->ukflag = 0;
	_dlg->ukflag2 = FA_GET_PV(DWORD, _items - 4);
	return _dlg;
}
