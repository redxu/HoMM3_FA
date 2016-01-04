#ifndef __HOOKAPI__H__
#define __HOOKAPI__H__

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

//ָ��API����HOOK��������һ������ԭ�����Ĵ�������ַ��
void* HookFunction(void* pSourceFunction,void* pNewFunction);

//�ָ���HOOK�ĺ���������ҪHookFunction�������صĴ�������ַ��Ϊ������
BOOL UnHookFunction(void* pSourceFunction,void* pProxyFunction);

//���ĳ�������Ƿ�HOOK
BOOL IsFuncHooked(void* pFunction);

#ifdef __cplusplus
}
#endif

#endif
