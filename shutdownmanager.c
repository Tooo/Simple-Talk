#include <stdlib.h>

#include "keyboard.h"
#include "sender.h"
#include "receiver.h"
#include "screen.h"

static bool isShutingDown = false;

// Block current thread until some other thread calls triggerShutdown() method
void ShutdownManager_waitForShutdown() {

}

// Indicate that we should shutdown, and allowing all threads which
// blocked on waitForShutdown() function to continue
void ShutdownManager_triggerShutdown() {
    Screen_shutdown();
    Keyboard_shutdown();
}

bool ShutdownManager_isShuttingDown() {
    return isShutingDown;
}