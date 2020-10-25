#include "keyboard.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "screen.h"
#define BUFFER 128

static pthread_t thread;

static List * inputList;
static char * message = NULL;

void * keyboardThread(void* unused) {
    while (1) {
        message = malloc(BUFFER);
        fgets(message, BUFFER, stdin);

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