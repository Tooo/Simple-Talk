#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "sleep.h"
#include "printer.h"
#include "signaller.h"

// return true if its computed average OK

static bool getAverage(double *pData, int size, double *pAvgOut) {

    if (size == 0) {
        return false;
    }

    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += * (pData + i);
    }
    *pAvgOut = sum / size;
    return true;
}

int main(int argc, char** args) {

    double myData[] = {1.0, 2.0};
    double myAverage = 0;
    if (getAverage(myData, 2, &myAverage)) {
        printf("Average is %f\n", myAverage);
    } else {
        printf("Could not computer average\n");
    }

    //Startup
    Printer_init();
    Signaller_init();

    // Cleanup
    Printer_waitForShutdown();
    Signaller_waitForShutdown();

    printf("\n\nDONE\n");
    return 0;
}