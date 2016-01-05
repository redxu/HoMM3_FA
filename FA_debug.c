#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <time.h>
#include "FA_debug.h"

/**
 * [Dump memory to TXT]
 * @param addr [memory addr]
 * @param sz   [memory size]
 */
void FA_DumpTxt(DWORD addr, int sz) {
	int i, len;
	BYTE* ptr = (BYTE *)addr;
	char mem[1024];
	memset(mem, 0, sizeof(mem));
	len = 0;
	for(i=0; i<sz; i++) {
		if(i%16 == 0) {
			len = sprintf(mem, "[%04X]", i);
		}

		len += sprintf(mem+len, "%02X ", *(ptr+i));
		if((i+1)%16 == 0) {
			FA_Log(mem);
			memset(mem, 0, sizeof(mem));
			len = 0;
		}
	}
	if(len != 0) {
		FA_Log(mem);
	}
}

/**
 * [Dump memory to BIN]
 * @param addr     [memory addr]
 * @param sz       [memory size]
 * @param filename [dumpfile]
 */
void FA_DumpBin(DWORD addr, int sz, const char* filename) {
	BYTE* ptr = (BYTE *)addr;
	FILE* file = NULL;
	file = fopen(filename, "wb+");
	if(file == NULL) {
		FA_Log("FA_DumpBin can't Create %s", filename);
		return;
	}

	fwrite(ptr, sz, 1, file);
	fclose(file);
	fflush(file);
}

/**
 * [Log msg]
 * @param format   [msg fmt]
 * @param ...      [va_args]
 */
void FA_Log(const char* format, ...) {
	char buf[4096];
	FILE *file = NULL;
	time_t ttime = 0;
	struct tm *tm = NULL;
	va_list ap;
	
	time(&ttime);
	tm = localtime(&ttime);
	
	file = fopen("./HoMM3_FA.Log","ab+");
	if(file == NULL) {
		OutputDebugString("Can't Create HoMM3_FA.Log!!");
		return;
	}
	
	sprintf(buf, "[%04d%02d%02d %02d:%02d:%02d]\t", 
		tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
		tm->tm_hour,tm->tm_min,tm->tm_sec);
	fwrite(buf,strlen(buf),1,file);
		
	memset(buf,0,sizeof(buf));
	va_start(ap,format);
	vsprintf(buf,format,ap);
	strcat(buf,"\r\n");
	va_end(ap);
	
	fwrite(buf,strlen(buf),1,file);
	
	fflush(file);
	fclose(file);	
}
