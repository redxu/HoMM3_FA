#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <windows.h>
#include <time.h>
#include "FA_log.h"


void FA_log(const char* format,...) {
	char buf[4096];
	FILE *file = NULL;
	time_t ttime = 0;
	struct tm *tm = NULL;
	va_list ap;
	
	time(&ttime);
	tm = localtime(&ttime);
	
	file = fopen("./HoMM3_FA.Log","ab+");
	if(file == NULL) {
		printf("无法创建日志文件\n");
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
