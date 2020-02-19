#include <stdio.h>
#include <stdlib.h>


/* print Fahrenheit-Celsius table */

int main(int argc, char **argv)
{

    if (argc == 2) {
        int fahr = atof(argv[1]);
        printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    } else if (argc == 4){
        const int LOWER = atoi(argv[1]);
        const int UPPER = atoi(argv[2]);
        const int STEP = atoi(argv[3]);

        int fahr;
        for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));
    }

    return 0;
}
