#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000 // max lines to be sorted
char *lineptr[MAXLINES]; // pointer to text line

// my generic merge sort (gmsort)
void gmsort(void *lineptr[], int left, int right,
            int (*comp) (void *, void *));
void merge(void * listptr[], int left, int middle, int right,
            int (*comp) (void *, void *));
int numcmp(char *, char *);
int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
int mygetline(char s[], int lim);

int main(int argc, char *argv[])
{
    int nlines; // number of lines read
    int numeric = 0; // 1 if numeric

    if (argc > 1 && strcmp(argv[1], "-n") == 0)
        numeric = 1;

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        gmsort((void **) lineptr, 0, nlines-1, 
            (int (*)(void*, void*)) (numeric ? numcmp : strcmp));
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
    return 0;
}

void gmsort(void *v[], int left, int right,
            int (*comp) (void *, void *))
{
    int middle;

    if (left >= right) // list has 1 or fewer values
        return;
    middle = (left + right) / 2;
    gmsort(v, left, middle, comp);
    gmsort(v, middle + 1, right, comp);
    merge(v, left, middle, right, comp);

}

void merge(void *v[], int l, int m, int r,
            int (*comp) (void *, void *))
{
    int p = l, q = m + 1;

    void **arr = malloc((r - l + 1) * sizeof(*arr));

    int k = 0;

    int i;

    for (i = l; i <= r; i++) {
        if (p > m) arr[k++] = v[q++];
        else if (q > r) arr[k++] = v[p++];
        else if ((*comp) (v[p], v[q]) < 0) arr[k++] = v[p++];
        else arr[k++] = v[q++];
    }

    for (i = 0; i < k; i++) {
        v[l++] = arr[i];
    }
    free(arr);
    
}

int numcmp(char *s1, char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else 
        return 0;
}

int readlines(char *lineptr[], int maxlines) {
    printf("Escriba palabras... (ctrl-d para terminar)\n");
    int len, nlines;
    char *p, line[MAXLINES];

    nlines = 0;
    while ((len = mygetline(line, MAXLINES)) > 0)
    {
        if (nlines >= maxlines || (p = malloc(len * sizeof(*p))) == NULL)
            return -1;
        else {
            line[len-1] = '\0'; // delete newline
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

void writelines(char *lineptr[], int nlines)
{
    printf("\n");
    while (nlines-- > 1)
        printf("%s, ", *lineptr++);
    printf("%s\n", *lineptr);
}

int mygetline(char s[], int lim)
{
    int c, i;

    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; i++)
        s[i] = c;
    if (c == '\n') {
        s[i] = c;
        i++;
    }
    s[i] = '\0';
    return i;

}
