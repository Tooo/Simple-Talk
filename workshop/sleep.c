#include <time.h>
#include "sleep.h"

void sleep_usec(long usec) {
    struct timespec sleep_time;
    sleep_time.tv_sec = (usec / 1000000);
    sleep_time.tv_nsec = (usec % 1000000) * 1000;
    nanosleep(&sleep_time, NULL);
}

void sleep_msec(long msec) {
    sleep_usec(msec * 1000);
}