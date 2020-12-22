#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "keyboard.h"
#include "sender.h"
#include "receiver.h"
#include "screen.h"
#include "listmanager.h"

/*
    Shutdown Manager Thread
    Locked when messaging system is active
    Starts when triggerShutdown is called
*/

static pthread_t thread;
static pthread_cond_t shutdownCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;

static bool isShutingDown = false;

void * shutdownThread(void * unused) {
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_wait(&shutdownCondVar, &shutdownMutex);
    }
    pthread_mutex_unlock(&shutdownMutex);

    Keyboard_clean();
    Sender_clean();
    Reciever_clean();
    Screen_clean();

    ListManager_free();
    return NULL;
}

void ShutdownManager_init() {
    pthread_create(&thread, NULL, shutdownThread, NULL);
}

void ShutdownManager_waitForShutdown() {
    Keyboard_waitForShutdown();
    Sender_waitForShutdown();
    Receiver_waitForShutdown();
    Screen_waitForShutdown();
    pthread_join(thread, NULL);
}

void ShutdownManager_triggerShutdown() {
    isShutingDown = true;
    pthread_mutex_lock(&shutdownMutex);
    {
        pthread_cond_signal(&shutdownCondVar);
    }
    pthread_mutex_unlock(&shutdownMutex);
}

bool ShutdownManager_isShuttingDown() {
    return isShutingDown;
}