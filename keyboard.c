#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "keyboard.h"
#include "screen.h"
#include "list.h"
#include "listmanager.h"

static pthread_t thread;

static List * inputList;
static char * message = NULL;

void * keyboardThread(void* unused) {
    while (1) {
        message = malloc(MAX_STRING_LEN);
        fgets(message, MAX_STRING_LEN, stdin);

        ListManager_lockInputList();
        List_prepend(inputList, message);
        ListManager_unlockInputList();

        Sender_signalNextMessage();

        if (strlen(message) == 2 && message[0] == '!') {
            return NULL;
        }
    }

    return NULL;
}

void Keyboard_init() {
    inputList = ListManager_getInputList;
    pthread_create(&thread, NULL, keyboardThread, NULL);
}

void Keyboard_waitForShutdown() {
    pthread_join(thread, NULL);
}

void Keyboard_clean() {
    free(message);
}