#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "keyboard.h"
#include "sender.h"
#include "list.h"
#include "listmanager.h"
#include "shutdownmanager.h"

/*
    Keyboard thread
    Receieve input from local computer
    Sends to sender thread
    (Refer to Brian Fraser Threads workshop)
*/

static pthread_t thread;

static List * inputList;
static char * message = NULL;

void * keyboardThread(void* unused) {
    while (!ShutdownManager_isShuttingDown()) {
        message = malloc(MAX_STRING_LEN);

        if (message == NULL) {
            puts("Keyboard: Fail to malloc message");
            ShutdownManager_triggerShutdown();
            break;
        }

        fgets(message, MAX_STRING_LEN, stdin);

        ListManager_lockInputList();
        if (List_prepend(inputList, message) == -1) {
            puts("Keyboard: Fail to prepend to list");
        }
        
        ListManager_unlockInputList();

        Sender_signalNextMessage();
    }

    return NULL;
}

void Keyboard_init() {
    inputList = ListManager_getInputList();
    pthread_create(&thread, NULL, keyboardThread, NULL);
}

void Keyboard_waitForShutdown() {
    pthread_join(thread, NULL);
}

void Keyboard_clean() {
    pthread_cancel(thread);
    if (message != NULL) {
        free(message);
    }
}