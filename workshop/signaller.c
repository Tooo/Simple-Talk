#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "printer.h"
#include "sleep.h"

pthread_t thread;

static const char * MESSAGE = "Hello world in Sync!";

void * singallerThread(void* unused) {
    for (const char * msg = MESSAGE; *msg != '\0'; msg++) {
        sleep_msec(500);
        
        // Signal other thread
        Printer_signalNextChar();
    }

    return NULL;
}

void Signaller_init() {
    pthread_create(&thread, NULL, singallerThread, NULL);
}

void Signaller_waitForShutdown() {
    pthread_join(thread, NULL);
}