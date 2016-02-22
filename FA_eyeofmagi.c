/*****************************************************************************
* File Descrip : Eye of the magi new function
* Create Time  ：20160126
* Author	   ：RedXu
*****************************************************************************/

#include "FA_def.h"
#include "FA_struct.h"
#include "FA_debug.h"
#include "H3_Function.h"
#include "FA_mod.h"

/**
 * [EyeDlg窗口过程]
 * @param  dlg [dlg指针]
 * @param  cmd [cmd指令]
 * @return     [0/1]
 */
static DWORD FA_THISCALL EyeDlgProc(struct H3_Dlg* dlg, struct H3_DlgItemCmd* cmd) {
	int rtv = H3_DlgProc(dlg, cmd);
	if(rtv)
	{
		H3_MessageBox("wrong!", 1, -1, 0, 0);
		return rtv;
	}
	
	switch(cmd->type) {
		case 0x200:
		FA_Log("mouse click! stype=%d id=%d param=%d", cmd->stype, cmd->itemid, cmd->param);
		if(cmd->itemid == 22)
			H3_MessageBox("Hello", 1, -1, 0, 0);
		break;
	}

	return 1;
}

static void FA_HeroVisitEyeOfMagi(void) {
	struct H3_DlgItemCmd cmd;
	struct H3_Dlg_VTable* vtable = FA_DlgVTableCreate();
	vtable->dlgproc = EyeDlgProc;
	struct H3_Dlg* _dlg_ = (struct H3_Dlg *)FA_DlgCreate(80, 60, 640, 480, 100, vtable);

	BYTE* _dlgitem_ = H3_Malloc(0x34);
	H3_DlgBuildPcxItem(_dlgitem_, 0, 0, 640, 480, 11, "heroscr4.pcx", 0x800);

	DWORD** list = (DWORD **)&_dlg_->itemlist;
	H3_DlgAddItem(list, list[2], 1, &_dlgitem_);


	//_dlgitem_ = H3_Malloc(0x48);
	//H3_DlgBuildDefItem(_dlgitem_, 300, 440, 0x2c, 38, 22, "iCANCEL.def", 0, 1, 0, 0, 0x10);
	_dlgitem_ = H3_Malloc(0x68);
	H3_DlgBuildDefItem2(_dlgitem_, 300, 440, 0x2c, 38, 22, "iCANCEL.def", 0, 1, 0, 0, 2);
	H3_DlgAddItem(list, list[2], 1, &_dlgitem_);

	_dlgitem_ = H3_Malloc(0x68);
	H3_DlgBuildDefItem2((BYTE *)_dlgitem_, 500, 440, 0x40, 0x1e, 0x7802, "iOkay.def", 0, 1, 1, 0, 2);
	H3_DlgAddItem(list, list[2], 1, &_dlgitem_);

	//init items
	DWORD *itemfirst,*itemlast;
	itemfirst = _dlg_->itemlist.first;
	itemlast = _dlg_->itemlist.last;
	while(itemfirst != itemlast) {
		H3_DlgInitItem(_dlg_, *itemfirst, -1);
		itemfirst++;
	}

	//send cmd.
/*
	memset(&cmd, 0, sizeof(cmd));
	cmd.type = 0x200;
	cmd.stype = 12;
	cmd.itemid = 22;
	cmd.param = 0;		//def index.
	H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
	cmd.stype = 13;
	//cmd.param = 4;
	H3_DlgSendCmd2Item((BYTE *)_dlg_, &cmd);
*/
	
	_dlg_->vtable->exec(_dlg_, 0);
}

//local mods for eyeofmagi
static struct FA_Mod __mods[] = {
	//Hero Visit EysOfMagi
	{FA_MOD_TYPE_CALL, 0x4A8B07, (DWORD)FA_HeroVisitEyeOfMagi, 0x2f},
};

/**
 * [FA_EyeOfMagi_Init]
 */
void FA_EyeOfMagi_Init(void) {
	int n = FA_ARRAYSIZE(__mods);
	FA_Mod_Register(__mods, n);
}

