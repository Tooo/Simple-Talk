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

        List_prepend(inputList, message);
        Screen_signalNextMessage();

        if (strlen(message) == 2 && message[0] == '!') {
            return NULL;
        }
    }

    return NULL;
}

void Keyboard_init(List * list) {
    inputList = list;
    pthread_create(&thread, NULL, keyboardThread, NULL);
}

void Keyboard_shutdown() {
    free(message);
    pthread_join(thread, NULL);
}