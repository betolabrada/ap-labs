#include <stdio.h>
#define MAXLINE 1000 /* maximum line size */

int getlinelen(char line[], int maxline);
void reverse(char line[], char rev[], int size);

int main(){

    int len;
    
    char line[MAXLINE];
    char rev[MAXLINE];

    printf("Word-Reverser in C by Alberto Labrada\n\n");
    while ((len = getlinelen(line, MAXLINE)) > 0) {
        reverse(line, rev, len);
        printf("%s\n\n", rev);
    }

    printf("Goodbye!!\n");

    return 0;
}

int getlinelen(char s[], int lim) {
    printf("Please write something (to exit press ctrl+d)... ");
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != '\n' && c != EOF; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        ++i;
    }
    s[i] = '\0';

    return i;
}

void reverse(char l[], char r[], int len) {

    int i, j;

    for (i = 0, j = len - 1; i < len && j >= 0; i++, j--)
        r[i] = l[j];
    r[i] = '\0';

}
