#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>

int loggertype = 0; // 0 is stdout (default) 1: syslog


int initLogger(char *logType) {
    if (strcmp(logType, "") == 0 || strcmp(logType, "stdout") == 0) {
        // type stdout 0
        loggertype = 0;
    } else if (strcmp(logType, "syslog") == 0){
        // type syslog 1
        loggertype = 1;
    } else {
        // error
        perror("please specify a type (stdout or syslog)");
        return 1;
    }
    printf("Initializing Logger on: %s\n", logType);
    return 0;
}

void ftime(char * buf, int size) {

	/* get current time */
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);

	
	buf[strftime(buf, size, "%H:%M:%S", lt)] = '\0';

}

int infof(const char *format, ...) {

    if (loggertype == 0) {
		char timebuf[16];

		ftime(timebuf, 16);	

		fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRINFO, "INFO", __FILE__, __LINE__);
	}

    va_list args; 
	
	va_start(args, format);

	if (loggertype == 0) vfprintf(stdout, format, args);
	else {
		openlog("INFO", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_INFO, format, args);
		closelog();
	}
	
	va_end(args);

	if (loggertype == 0) {
		fprintf(stdout, "\n");

		fflush(stdout);
	}

	return 0;
}

int warnf(const char *format, ...) {

    if (loggertype == 0) {
		char timebuf[16];

		ftime(timebuf, 16);	

		fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRWARN, "WARN", __FILE__, __LINE__);
	}

    va_list args; 
	
	va_start(args, format);

	if (loggertype == 0) vfprintf(stdout, format, args);
	else {
		openlog("WARN", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_WARNING, format, args);
		closelog();
	}
	
	va_end(args);

	if (loggertype == 0) {
		fprintf(stdout, "\n");

		fflush(stdout);
	}

	return 0;
}

int errorf(const char *format, ...) {

	if (loggertype == 0) {
		char timebuf[16];

		ftime(timebuf, 16);	

		fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRERROR, "ERROR", __FILE__, __LINE__);
	}

    va_list args; 
	
	va_start(args, format);

	if (loggertype == 0) vfprintf(stdout, format, args);
	else {
		openlog("ERROR", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_ERR, format, args);
		closelog();
	}
	
	va_end(args);

	if (loggertype == 0) {
		fprintf(stdout, "\n");

		fflush(stdout);
	}

	return 0;
}

int panicf(const char *format, ...) {

	if (loggertype == 0) {
		char timebuf[16];

		ftime(timebuf, 16);	

		fprintf(stdout, "\x1b[90m%s %s%-6s\x1b[90m%s:%d:\x1b[0m ", 
			timebuf, CLRPANIC, "PANIC", __FILE__, __LINE__);
	}

    va_list args; 
	
	va_start(args, format);

	if (loggertype == 0) vfprintf(stdout, format, args);
	else {
		openlog("PANIC", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_EMERG, format, args);
		closelog();
	}
	
	va_end(args);

	if (loggertype == 0) {
		fprintf(stdout, "\n");

		fflush(stdout);
	}

	raise(SIGABRT);
	return 1;
}

