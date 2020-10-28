#include "printer.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sleep.h"

pthread_t threadPrinter;

static const char * MESSAGE = "Hello world in Sync!";

static pthread_cond_t s_syncOkToPrintCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_syncOkToPrintMutex = PTHREAD_MUTEX_INITIALIZER;

void * printerThread(void* unused) {
    for (const char * msg = MESSAGE; *msg != '\0'; msg++) {

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

void Printer_signalNextChar() {
    pthread_mutex_lock(&s_syncOkToPrintMutex);
    {
        pthread_cond_signal(&s_syncOkToPrintCondVar);
    }
    pthread_mutex_unlock(&s_syncOkToPrintMutex);
}

void Printer_init() {
    pthread_create(&threadPrinter, NULL, printerThread, NULL);
}

void Printer_waitForShutdown() {
    pthread_join(threadPrinter, NULL);
}
