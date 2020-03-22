#include <stdio.h>
#include <stdlib.h>

int mystrlen(char *str){

    int c;
    for (c = 0; str[c] != '\0'; c++)
        ;
    return c;
}

char *mystradd(char *origin, char *addition){

    int i = 0;
    char * newstr = malloc((mystrlen(origin) + mystrlen(addition) + 1) * sizeof(*newstr));

    while (*origin != '\0')
        newstr[i++] = *origin++;
    while(*addition != '\0')
        newstr[i++] = *addition++;
    newstr[i] = '\0';

    return newstr;
}

// 1: found, 0: not found
int mystrfind(char *origin, char *substr){

    while (*origin != '\0')
    {
        if (*origin == *substr && *substr != '\0')
            substr++;
        origin++;
    }
    
    if (*substr == '\0') return 1;

    return 0;
}
