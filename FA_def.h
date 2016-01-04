#ifndef __FA_DEF__H__
#define __FA_DEF__H__

#include <stdint.h>

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD uint16_t
#endif

/**   MOD Type **/
#define FA_MOD_TYPE_CALL	0
#define FA_MOD_TYPE_JUMP	1

#define FA_ARRAYSIZE(T)		(sizeof(T)/sizeof((T)[0]))


#define FA_EXPORT	__declspec(dllexport)

/** Call Convention */
#if defined(__GNUC__)
#define FA_FASTCALL	__attribute__((fastcall))
#define FA_STDCALL 	__attribute__((stdcall))
#define FA_CDECL	__attribute__((cdecl))
#elif defined(_MSC_VER)
#define FA_FASTCALL __fastcall
#define FA_STDCALL	__stdcall
#define FA_CDECL	__cdecl
#endif


/** FA SOD DEFINE **/
//基址
#define FA_ADDR_BASE        0x699538

//GAMETYPE
#define FA_ADDR_GAMETYPE	0x698A40
#define FA_GAMETYPE_SINGLE	0
#define FA_GAMETYPE_HOTSEAT	3

#endif

