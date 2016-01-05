#ifndef __FA_DEBUG__H__
#define __FA_DEBUG__H__

#include <windows.h>

/**
 * [Dump memory to TXT]
 * @param addr [memory addr]
 * @param sz   [memory size]
 */
void FA_DumpTxt(DWORD addr, int sz);

/**
 * [Dump memory to BIN]
 * @param addr     [memory addr]
 * @param sz       [memory size]
 * @param filename [dumpfile]
 */
void FA_DumpBin(DWORD addr, int sz, const char* filename);

/**
 * [Log msg]
 * @param format   [msg fmt]
 * @param ...      [va_args]
 */
void FA_Log(const char* format, ...);


#endif
