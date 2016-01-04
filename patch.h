#ifndef __PATCH__H__
#define __PATCH__H__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL PatchCode(PVOID addr,u_char* code,SIZE_T len);


#ifdef __cplusplus
}
#endif

#endif
