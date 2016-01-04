#include <windows.h>
#include "FA_lua.h"
#include "FA_mod.h"
#include "FA_Detour.h"


/**
 * [FA_Mod EP]
 */
static void FA_Mod(void){
	//init Lua
	FA_lua_init();
	//hack mod
	FA_Mod_Init();
    //detour mod
    FA_Detour_Init();
}

static void FA_UnMod(void) {
	//close Lua
	FA_lua_close();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
        	FA_Mod();
            break;

        case DLL_PROCESS_DETACH:
        	FA_UnMod();
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
