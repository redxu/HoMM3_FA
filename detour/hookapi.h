#ifndef __HOOKAPI__H__
#define __HOOKAPI__H__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

//指定API进行HOOK，并返回一个进入原函数的代理函数地址。
void* HookFunction(void* pSourceFunction,void* pNewFunction);

//恢复被HOOK的函数，它需要HookFunction函数返回的代理函数地址作为参数。
BOOL UnHookFunction(void* pSourceFunction,void* pProxyFunction);

//检查某个函数是否被HOOK
BOOL IsFuncHooked(void* pFunction);

#ifdef __cplusplus
}
#endif

#endif
