#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIGMONTH 31
#define SMLMONTH 30
#define FEBRAURY 28
#define FEBLEAP  29
#define MONTHS   12
#define STRMONTH 4

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday);

/* returns 1 if the year is leap year, 0 if it is not */
int leap_year(int year);

/* constructing month array */
void getdayspermonth(int leap);

/* month number to text */
void convert_month(int month, char *string);

static int dayspermonth[MONTHS + 1];

int main(int argc, char **argv) {

    int year, day;
    char strmonth[STRMONTH];

    year = atoi(argv[1]);
    day = atoi(argv[2]);

    int leap = leap_year(year);
    getdayspermonth(leap);

    if (!(1 <= day  && day <= 365 + leap)) {
        printf("Please choose a valid day number.\n");
        printf("Exiting...\n");
        return 1;
    }

    int a, b;
    int *pmonth, *pday;
    pmonth = &a;
    pday = &b;

    month_day(year, day, pmonth, pday);

    convert_month(*pmonth, strmonth);

    printf("%s %02d, %d\n", strmonth, *pday, year);


    return 0;
}

void month_day(int y, int yd, int *pm, int *pd) {
    
    int i;

    for (i = 1; yd > dayspermonth[i]; i++) {
        yd -= dayspermonth[i];
    }
    *pm = i;
    *pd = yd;
    
}

int leap_year(int year) {
    return year % 4 == 0 && !(year % 100 == 0 && year % 400 != 0);
}

void getdayspermonth(int b) {
    int i;

    for (i = 1; i <= MONTHS; i++) {
        if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12) 
            dayspermonth[i] = BIGMONTH;
        else if (i == 4 || i == 6 || i == 9 || i == 11) 
            dayspermonth[i] = SMLMONTH;
        else 
            dayspermonth[i] = !b ? FEBRAURY : FEBLEAP;
    }
}

void convert_month(int m, char *s) {

    switch (m)
    {
    case 1:
        strcpy(s, "Jan");
        break;
    
    case 2:
        strcpy(s, "Feb");
        break;
    
    case 3:
        strcpy(s, "Mar");
        break;
    
    case 4:
        strcpy(s, "Apr");
        break;
    
    case 5:
        strcpy(s, "May");
        break;
    
    case 6:
        strcpy(s, "Jun");
        break;
    
    case 7:
        strcpy(s, "Jul");
        break;
    
    case 8:
        strcpy(s, "Ago");
        break;
    
    case 9:
        strcpy(s, "Sep");
        break;
    
    case 10:
        strcpy(s, "Oct");
        break;
    
    case 11:
        strcpy(s, "Nov");
        break;
    
    case 12:
        strcpy(s, "Dec");
        break;
    
    default:
        break;
    }
    
}
