#ifndef _SHUTDOWNMANAGER_H_
#define _SHUTDOWNMANAGER_H_

#include <stdbool.h>

void ShutdownManager_init();

// Block current thread until some other thread calls triggerShutdown() method
void ShutdownManager_waitForShutdown();

// Indicate that we should shutdown, and allowing all threads which
// blocked on waitForShutdown() function to continue
void ShutdownManager_triggerShutdown();

bool ShutdownManager_isShuttingDown();

#endif