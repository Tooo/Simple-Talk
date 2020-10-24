#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sleep.h"

static const char * MESSAGE = "Hello world in Sync!";

// Synchronization
static pthread_cond_t s_syncOkToPrintCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToPrintMutex = PTHREAD_MUTEX_INITIALIZER;

void * printerThread(void* unused) {
    for (const char * msg = MESSAGE; *msg != '\0'; msg++) {

        // Wait until signal
        pthread_mutex_lock(&s_syncOkToPrintMutex);
        {
            pthread_cond_wait(&s_syncOkToPrintCondVar, &s_syncOkToPrintMutex);
        }
        
        pthread_mutex_unlock(&s_syncOkToPrintMutex);

        printf("%c", *msg);
        fflush(stdout);
    }

    return NULL;
}

void * singallerThread(void* unused) {
    for (const char * msg = MESSAGE; *msg != '\0'; msg++) {
        sleep_msec(500);
        
        // Signal other thread
        pthread_mutex_lock(&s_syncOkToPrintMutex);
        {
            pthread_cond_signal(&s_syncOkToPrintCondVar);
        }
        
        pthread_mutex_unlock(&s_syncOkToPrintMutex);
    }

    return NULL;
}

int main(int argc, char** args) {
    pthread_t threadPrinter;
    pthread_t threadSignaller;

    // Start Thread
    pthread_create(&threadPrinter, NULL, printerThread, NULL);
    pthread_create(&threadSignaller, NULL, singallerThread, NULL);

    pthread_join(threadPrinter, NULL);
    pthread_join(threadSignaller, NULL);

    printf("\n\nDONE\n");
    return 0;
}