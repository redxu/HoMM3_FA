/*****************************************************************************
* File Descrip : FA Dlg Function Wrap
* Create Time  ：20160226
* Author	   ：RedXu
*****************************************************************************/

#ifndef __FA_DLG__H__
#define __FA_DLG__H__

/**
 * [Create Dlg Default VTable]
 * @return  [VTable]
 */
struct H3_Dlg_VTable* FA_DlgVTableCreate(void);

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
struct H3_Dlg* FA_DlgCreate(int x, int y, int dx, int dy, int count, struct H3_Dlg_VTable* vt);

#endif
