#include <stdio.h>
#include <stdlib.h>

#include "shutdownManager.h"

#include "keyboard.h"
#include "sender.h"
#include "receiver.h"
#include "screen.h"

int main (int arc, char** args) {

    if (arc != 4) {
        printf("Invalid Arguments\n");
        printf("Sample: s-talk 9001 csil-cpu6.csil.sfu.ca 8989\n");
        return 1;
    }

    int inputPort = args[1];
    char * ipAddress = args[2];
    int outputPort = args[3];

    Keyboard_init();
    Sender_init(ipAddress, outputPort);
    Receiver_init(inputPort);
    Screen_init();
    
    ShutdownManager_waitForShutdown();

    return 0;
}