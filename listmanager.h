#ifndef _LISTMANAGER_H_
#define _LISTMANAGER_H_

#include "list.h"

// Create input and output Lists
int ListManager_create();

// Free input and output Lists
void ListManager_free();

List * ListManager_getInputList();
List * ListManager_getOutputList();

void ListManager_lockInputList();
void ListManager_unlockInputList();

void ListManager_lockOutputList();
void ListManager_unlockOutputList();

#endif