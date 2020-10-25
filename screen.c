#include "screen.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

static pthread_t thread;
static pthread_cond_t screenCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t screenMutex = PTHREAD_MUTEX_INITIALIZER;

static List * outputList;
static char * message = NULL;

void * screenThread(void* unused) {
    while (1) {
        pthread_mutex_lock(&screenMutex);
        {
            pthread_cond_wait(&screenCondVar, &screenMutex);
        }
        pthread_mutex_unlock(&screenMutex);
        message = List_trim(outputList);

        if (strlen(message) == 2 && message[0] == '!') {
            return NULL;
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

void Screen_init(List * list) {
    outputList = list;
    pthread_create(&thread, NULL, screenThread, NULL);
}

void Screen_shutdown() {
    free(message);
    pthread_join(thread, NULL);
}