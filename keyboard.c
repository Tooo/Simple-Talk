#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "keyboard.h"
#include "sender.h"
#include "list.h"
#include "listmanager.h"
#include "shutdownmanager.h"

static pthread_t thread;

static List * inputList;
static char * message = NULL;

void * keyboardThread(void* unused) {
    while (!ShutdownManager_isShuttingDown()) {
        puts("In keyboard loop");
        message = malloc(MAX_STRING_LEN);
        fgets(message, MAX_STRING_LEN, stdin);

        ListManager_lockInputList();
        List_prepend(inputList, message);
        ListManager_unlockInputList();

        Sender_signalNextMessage();

        if (strlen(message) == 2 && message[0] == '!') {     
            ShutdownManager_triggerShutdown();
            return NULL;
        }
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
    if (!message) {
        free(message);
    }
}