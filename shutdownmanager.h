#ifndef _SHUTDOWNMANAGER_H_
#define _SHUTDOWNMANAGER_H_

#include <stdbool.h>

void ShutdownManager_init();
void ShutdownManager_waitForShutdown();
void ShutdownManager_triggerShutdown();
bool ShutdownManager_isShuttingDown();

#endif