Lab - Logger Library
====================

Implement a Logger library in C that will be used for printing the following types of messages.
- `INFO` General information.
- `WARN` Warnings.
- `ERROR` Errors.
- `PANIC` Panics. Non recoverable issues with core dump.

This `logger` library will be used instead of `printf` function calls in all new labs and challenges that we'll be doing in the course.
Below you can see the prototype for each function.

```
int infof(const char *format, ...)
int warnf(const char *format, ...)
int errorf(const char *format, ...)
int panicf(const char *format, ...)
```

General instructions
--------------------
- Use the `logger.c` file for your implementation.
- Update `README.md` with the proper steps for building and running your code.
- Use the `testLogger.c` file for testing all your logger functions.
- Use different font color for each of the log types. For colors references go to: https://www.linuxjournal.com/article/8603
- Don't forget to handle errors properly.
- Coding best practices implementation will be also considered.

How to submit your work
=======================
```
GITHUB_USER=<your_github_user>  make submit
```
More details at: [Classify API](../../classify.md)


How to run Logger
=================
### Include the files `logger.h` and `logger.c` into your project.
```
infof("testing infof: there are %d melons", 3);
warnf("testing warnf: there are %d pineapples", 4);
errorf("testing errorf: there are %d apples", 10);
panicf("testing panicf: there are %d grapes", 500);
```

### Compile & Run Logger:
```
$ gcc -c logger.c -o logger.o
$ gcc -c testLogger.c -o testLogger.o
$ gcc testLogger.o logger.o -o testLogger
$ ./testLogger
```

### Output:
```
13:03:24 INFO  logger.c:30: testing infof: there are 3 melons
13:03:24 WARN  logger.c:53: testing warnf: there are 4 pineapples
13:03:24 ERROR logger.c:75: testing errorf: there are 10 apples
13:03:24 PANIC logger.c:97: testing panicf: there are 500 grapes
```


