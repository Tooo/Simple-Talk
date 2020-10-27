#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "keyboard.h"
#include "sender.h"
#include "receiver.h"
#include "screen.h"
#include "listmanager.h"

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

// Block current thread until some other thread calls triggerShutdown() method
void ShutdownManager_waitForShutdown() {
    Keyboard_waitForShutdown();
    Sender_waitForShutdown();
    Receiver_waitForShutdown();
    Screen_waitForShutdown();
    pthread_join(thread, NULL);
}

// Indicate that we should shutdown, and allowing all threads which
// blocked on waitForShutdown() function to continue
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