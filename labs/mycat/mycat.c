#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// MYCAT
void mycopy(int fd1)
{
    char buf[BUFSIZ];
    int n;

    while ((n = read(fd1, buf, BUFSIZ)) > 0)
        write(1, buf, n);
    
}

int main(int argc, char **argv)
{
    int fd;
    void mycopy(int);
    char * prog = argv[0];

    if (argc == 1) 
        mycopy(0); // copy input to output
    else
        while (--argc > 0)
            if ((fd = open(*++argv, O_RDONLY)) < 0) {
                fprintf(stderr, "%s: can′t open %s\n", prog, *argv);
                return 1;
            } else {
                mycopy(fd);
                close(fd);
            }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    return 0;
}