/* colors */
#define CLRINFO "\x1b[32m" /* green */
#define CLRWARN "\x1b[33m"  /* yellow */
#define CLRERROR "\x1b[31m"  /* red */
#define CLRPANIC "\x1b[35m"  /* magenta */

int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);



