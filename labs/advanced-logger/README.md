Lab  - Advanced Logger
======================

Now it's time to add support for Linux system logging into your logger that you implemented on [logger](https://github.com/CodersSquad/ap-labs/tree/master/labs/logger).

A new `initLogger`function will be added in order to add support for choosing between `stdout` or `syslog` logging.

```
int initLogger(char *logType)
```

You will handle the following log types:
- `stdout` or empty string for `STDOUT` logging
- `syslog` for `SYSLOG` logging

You can use the **The Linux Programming Interface** book as a reference for your implementation. See *37th chapter on 5th section*.

General Instructions
--------------------
- Use the `testLogger.c` file for testing your `logger.c` library implementation.
- You can update the `Makefile` for your compilation and linking.
- Update `README.md` with the proper steps for building and running your code.
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
```C
// default logging
infof("INFO Message %d", 1);
warnf("WARN Message %d", 2);
errorf("ERROR Message %d", 2);

// stdout logging
initLogger("stdout");
infof("INFO Message %d", 1);
warnf("WARN Message %d", 2);
errorf("ERROR Message %d", 2);

// syslog logging
initLogger("syslog");
infof("INFO Message %d", 1);
warnf("WARN Message %d", 2);
errorf("ERROR Message %d", 2);
```

### Compile & Run Logger:
```
$ gcc -c logger.c -o logger.o
$ gcc -c testLogger.c -o testLogger.o
$ gcc testLogger.o logger.o -o testLogger
$ ./testLogger
```

### Output (stdout):
```
11:32:37 INFO  logger.c:47: INFO Message 1
11:32:37 WARN  logger.c:80: WARN Message 2
11:32:37 ERROR logger.c:113: ERROR Message 2
Initializing Logger on: stdout
11:32:37 INFO  logger.c:47: INFO Message 1
11:32:37 WARN  logger.c:80: WARN Message 2
11:32:37 ERROR logger.c:113: ERROR Message 2
Initializing Logger on: syslog 
```

### Syslog output can be checked in /var/log/syslog file:
```
$ cat /var/log/syslog
...
Apr 17 11:32:37 ALBERTO-PC INFO[6903]: INFO Message 1                                                                   
Apr 17 11:32:37 ALBERTO-PC WARN[6903]: WARN Message 2                                                                   
Apr 17 11:32:37 ALBERTO-PC ERROR[6903]: ERROR Message 2
...
```
