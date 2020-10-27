#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "screen.h"
#include "list.h"
#include "listmanager.h"
#include "shutdownmanager.h"

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

        ListManager_lockOutputList();
        message = List_trim(outputList);
        ListManager_unlockOutputList();

        if (strlen(message) == 2 && message[0] == '!') {
            break;
        }

        puts(message);
        free(message);
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
    free(message);
}