#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "screen.h"
#include "list.h"
#include "listmanager.h"
#include "shutdownmanager.h"

/*
    Screen thread
    Receieve messages from receiver
    Prints messages
*/

static pthread_t thread;
static pthread_cond_t screenCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t screenMutex = PTHREAD_MUTEX_INITIALIZER;

static List * outputList;
static char * message = NULL;

void * screenThread(void* unused) {
    while (!ShutdownManager_isShuttingDown()) {
        pthread_mutex_lock(&screenMutex);
        {
            pthread_cond_wait(&screenCondVar, &screenMutex);
        }
        pthread_mutex_unlock(&screenMutex);

        if (ShutdownManager_isShuttingDown()) {
            break;
        }

        ListManager_lockOutputList();
        message = List_trim(outputList);
        ListManager_unlockOutputList();

        if (message == NULL) {
            puts("Screen: message is NULL");
        }

        fputs("Receiver: ", stdout);
        fputs(message, stdout);

        if (strlen(message) == 2 && message[0] == '!') {
            ShutdownManager_triggerShutdown();
            break;
        }

        if (message != NULL) {
            free(message);
            message = NULL;
        }
    }

    return NULL;
}

void Screen_signalNextMessage() {
    pthread_mutex_lock(&screenMutex);
    {
        pthread_cond_signal(&screenCondVar);
    }
    pthread_mutex_unlock(&screenMutex);
}

void Screen_init() {
    outputList = ListManager_getOutputList();
    pthread_create(&thread, NULL, screenThread, NULL);
}

void Screen_waitForShutdown() {
    pthread_join(thread, NULL);
}

void Screen_clean() {
    pthread_cancel(thread);
    pthread_mutex_destroy(&screenMutex);
    pthread_cond_destroy(&screenCondVar);

    if (message != NULL) {
        free(message);
    }
}