#include "patch.h"

BOOL PatchCode(PVOID addr,u_char* code,SIZE_T len)
{
	DWORD oldprotect;
	BOOL rtv;
	SIZE_T outlen;
	rtv = VirtualProtect(addr,len,PAGE_EXECUTE_READWRITE,&oldprotect);
	if(rtv != TRUE)
	{
		OutputDebugString("VirtualProtect fail");
		return rtv;
	}

	rtv = WriteProcessMemory(GetCurrentProcess(),addr,code,len,&outlen);
	if(rtv != TRUE)
	{
		OutputDebugString("WriteProcessMemory fail");
		return rtv;
	}
	if(outlen != len)
	{
		OutputDebugString("WriteProcessMemory len fail");
		return rtv;
	}

	VirtualProtect(addr,len,oldprotect,&oldprotect);

	return rtv;
}

