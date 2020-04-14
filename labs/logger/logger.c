#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>


void ftime(char * buf, int size) {

	/* get current time */
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);

	
	buf[strftime(buf, size, "%H:%M:%S", lt)] = '\0';

}


int infof(const char *format, ...) {

    va_list args; 
	
	char timebuf[16];

	ftime(timebuf, 16);	

	fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRINFO, "INFO", __FILE__, __LINE__);
	
	va_start(args, format);

	vfprintf(stdout, format, args);
	
	va_end(args);

	fprintf(stdout, "\n");

	fflush(stdout);

}

int warnf(const char *format, ...) {

    va_list args; 
	
	char timebuf[16];

	ftime(timebuf, 16);	

	fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRWARN, "WARN", __FILE__, __LINE__);
	
	va_start(args, format);

	vfprintf(stdout, format, args);
	
	va_end(args);

	fprintf(stdout, "\n");

	fflush(stdout);
}

int errorf(const char *format, ...) {

   	va_list args; 
	
	char timebuf[16];

	ftime(timebuf, 16);	

	fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRERROR, "ERROR", __FILE__, __LINE__);
	
	va_start(args, format);

	vfprintf(stdout, format, args);
	
	va_end(args);

	fprintf(stdout, "\n");

	fflush(stdout);
}

int panicf(const char *format, ...) {

    va_list args; 
	
	char timebuf[16];

	ftime(timebuf, 16);	

	fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRPANIC, "PANIC", __FILE__, __LINE__);
	
	va_start(args, format);

	vfprintf(stdout, format, args);
	
	va_end(args);

	fprintf(stdout, "\n");

	fflush(stdout);
}

