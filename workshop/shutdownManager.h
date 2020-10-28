
// Block current thread until some other thread calls triggerShutdown() method
void ShutdownManager_waitForShutdown();

// Indicate that we should shutdown, and allowing all threads which
// blocked on waitForShutdown() function to continue
void ShutdownManager_triggerShutdown();
bool ShutdownManager_isShuttingDown();