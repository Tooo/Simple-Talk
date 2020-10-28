#include <pthread.h>
#include <stdlib.h>

#include "list.h"

static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t outputMutex = PTHREAD_MUTEX_INITIALIZER;

static List * inputList;
static List * outputList;

// Create input and output Lists
int ListManager_create() {
    inputList = List_create();
    if (inputList == NULL) {
        return 1;
    }

    outputList = List_create();
    if (outputList == NULL) {
        return 2;
    }
    return 0;
}

// Free item for List
void freeItem(void * item) {
    free(item);
}

// Free input and output Lists
void ListManager_free() {
    List_free(inputList, freeItem);
    List_free(outputList, freeItem);
    pthread_mutex_destroy(&inputMutex);
    pthread_mutex_destroy(&outputMutex);
}

// Get input and output Lists
List * ListManager_getInputList() {
    return inputList;
}

List * ListManager_getOutputList() {
    return outputList;
}

// Lock and unlock inputList
void ListManager_lockInputList() {
    pthread_mutex_lock(&inputMutex);
}

void ListManager_unlockInputList() {
    pthread_mutex_unlock(&inputMutex);
}

// Lock and unlock outputList
void ListManager_lockOutputList() {
    pthread_mutex_lock(&outputMutex);
}

void ListManager_unlockOutputList() {
    pthread_mutex_unlock(&outputMutex);
}