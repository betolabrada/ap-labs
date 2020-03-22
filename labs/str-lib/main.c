#include <stdio.h>

int mystrlen(char *);
char * mystradd(char *, char *);
int mystrfind(char *str, char *substr);

int main(int argc, char **argv) {

    if (argc < 4) {
        printf("Usage: ./main originalVeryLongString Addition Add\n");
        return 1;
    }

    printf("%-25s : %-25d\n", "Initial Length ", mystrlen(argv[1]));
    char * newString = mystradd(argv[1], argv[2]);
    printf("%-25s : %-25s\n", "New string ", newString);
    printf("%-25s : %-25s\n", "Substring was found ", mystrfind(newString, argv[3]) ? "yes" : "no");

    return 0;
}
