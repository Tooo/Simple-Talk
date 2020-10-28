#ifndef _LISTMANAGER_H_
#define _LISTMANAGER_H_

#include "list.h"

#define MAX_STRING_LEN 256

// Create input and output Lists
int ListManager_create();

// Free input and output Lists
void ListManager_free();

// Get input and output Lists
List * ListManager_getInputList();
List * ListManager_getOutputList();

// Lock and unlock inputList
void ListManager_lockInputList();
void ListManager_unlockInputList();

// Lock and unlock outputList
void ListManager_lockOutputList();
void ListManager_unlockOutputList();

#endif