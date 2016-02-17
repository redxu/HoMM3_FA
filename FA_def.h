#ifndef __FA_DEF__H__
#define __FA_DEF__H__

#include <stdint.h>
#include <windef.h>

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD uint16_t
#endif

/**   MOD Type **/
#define FA_MOD_TYPE_CALL	0
#define FA_MOD_TYPE_JUMP	1
#define FA_MOD_TYPE_WRITE	2
#define FA_MOD_TYPE_NOP		3
#define FA_MOD_TYPE_BYTE	4
#define FA_MOD_TYPE_DETOUR	10

#define FA_ARRAYSIZE(T)		(sizeof(T)/sizeof((T)[0]))
#define FA_VERSION			"0.1alpha"
#define FA_MAXPATH			1024

#define FA_EXPORT	__declspec(dllexport)

/** Call Convention */
#if defined(__GNUC__)
#define FA_FASTCALL	__attribute__((fastcall))
#define FA_STDCALL 	__attribute__((stdcall))
#define FA_CDECL	__attribute__((cdecl))
#define FA_THISCALL	__attribute__((thiscall))
#define FA_INLINE	inline
#elif defined(_MSC_VER)
#define FA_FASTCALL __fastcall
#define FA_STDCALL	__stdcall
#define FA_CDECL	__cdecl
#define FA_THISCALL __thiscall
#define FA_NAKED	_declspec(naked)
#endif


/** FA SOD DEFINE **/
//基址
#define FA_ADDR_BASE        		0x699538

//Hero Levelup Exp After 12
#define FA_ADDR_HERO_LVUPEXP12		0x63AC20
#define FA_ADDR_SSKILL_NAME_BASE	0x67DCF0
#define FA_ADDR_SSKILL_LV_NAME_BASE 0x6A75D4

//GAMETYPE
#define FA_ADDR_GAMETYPE			0x698A40
#define FA_GAMETYPE_SINGLE			0
#define FA_GAMETYPE_HOTSEAT			3
//MessageBox
#define FA_MESSAGEBOX_YES			0x7805
#define FA_MESSAGEBOX_NO			0x7806

//Sec skills
#define FA_SSKILL_MYSTICISM_TABLE	0x63E9C8
#define FA_SSKILL_OFFENCE_TABLE		0x63E9F8
#define FA_SSKILL_ARMORER_TABLE		0x63EA08
#define FA_SSKILL_RESISTANCE_TABLE	0x63EA48

//DLG
#define FA_DLG_BUYBOAT_VTABLE		0x643918
#define FA_DLG_HEROINFO_VTABLE		0x63eAE8
#define FA_DLG_SYSOPTIONS_VTABLE	0x642478

//LOD 最大LOD数量
#define FA_MAX_LOD					8

//Artifact 最大宝物数量
#define H3_MAX_ARTIFACT				0x90
#define FA_MAX_ARTIFACT				255
//ComboArtifact 最大组合宝物数量
#define H3_MAX_COMBOARTIFACT		12
#define FA_MAX_COMBOARTIFACT		48






/** FA ASM FUNCTION **/
//Get Register Point
#if defined(__GNUC__)
#define FA_EAX(val)	 	__asm__ __volatile__ ("movl %%eax, %0" : "=m"(val));
#define FA_EBX(val)	 	__asm__ __volatile__ ("movl %%ebx, %0" : "=m"(val));
#define FA_ECX(val)	 	__asm__ __volatile__ ("movl %%ecx, %0" : "=m"(val));
#define FA_EDX(val)	 	__asm__ __volatile__ ("movl %%edx, %0" : "=m"(val));
#define FA_EBP(val)	 	__asm__ __volatile__ ("movl %%ebp, %0" : "=m"(val));
#define FA_EDI(val)	 	__asm__ __volatile__ ("movl %%edi, %0" : "=m"(val));
#define FA_ESI(val)	 	__asm__ __volatile__ ("movl %%esi, %0" : "=m"(val));
#define FA_SET_EAX(val)	__asm__ __volatile__ ("movl %0, %%eax" : "=m"(val));
#define FA_SET_ECX(val)	__asm__ __volatile__ ("movl %0, %%ecx" : "=m"(val));
//jmp use register ebx
#define FA_JMP_B(addr, esp)  		\
		do{							\
		DWORD __jmp_addr_ = addr; 	\
		__asm __volatile__ (		\
			"movl %0, %%ebx \n"		\
			: "=m"(__jmp_addr_)		\
		);							\
		__asm __volatile__ (		\
			"leave \n"				\
		);							\
		__asm __volatile__ (		\
			"add $"#esp", %esp \n"	\
		);							\
		__asm __volatile__ (		\
			"jmp *%ebx \n"			\
		);							\
		}while(0)
//jmp use register edx(recomend)
#define FA_JMP_D(addr, esp)  		\
		do{							\
		DWORD __jmp_addr_ = addr; 	\
		__asm __volatile__ (		\
			"movl %0, %%edx \n"		\
			: "=m"(__jmp_addr_)		\
		);							\
		__asm __volatile__ (		\
			"leave \n"				\
		);							\
		__asm __volatile__ (		\
			"add $"#esp", %esp \n"	\
		);							\
		__asm __volatile__ (		\
			"jmp *%edx \n"			\
		);							\
		}while(0)
#elif defined(_MSC_VER)
#define FA_EAX(val) 	__asm mov val, eax
#define FA_EBX(val) 	__asm mov val, ebx
#define FA_ECX(val) 	__asm mov val, ecx
#define FA_EDX(val) 	__asm mov val, edx
#define FA_ESI(val) 	__asm mov val, esi
#define FA_EDI(val) 	__asm mov val, edi
#define FA_EBP(val) 	__asm mov val, ebp
#define FA_SET_EAX(val)	__asm mov eax, val
#endif

//Get Value From Point Macro not support string
#define FA_GET_PV(type, point)	(*((type *)(point)))
//Save Value To Point Macro not support string
#define FA_SET_PV(type, point, val) (*((type *)(point)) = (type)val)

#endif

