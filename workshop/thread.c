#include <stdio.h>
#include "sleep.h"
#include <string.h>
#include <pthread.h>

void* slowPrintThread(void * msgAsVoid) {
    char* msg = msgAsVoid;
    for (int i = 0; i < strlen(msg); i++) {
        printf("%c", msg[i]);
        fflush(stdout);
        sleep_msec(200);
    }
    return NULL;
}

int main (int arc, char** args) {
    printf("**** DONE PROGRAM *****\n");

    pthread_t threadPID;
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        slowPrintThread,    // Functions
        "This is the new arg\n");              // Arguments

    pthread_t threadPID2;
    pthread_create(
        &threadPID2,         // PID (by pointer)
        NULL,               // Attributes
        slowPrintThread,    // Functions
        "*********************oops\n");              // Arguments

    pthread_join(threadPID, NULL);
    pthread_join(threadPID2, NULL);

    return 0;
}