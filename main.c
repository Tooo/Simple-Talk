#include <stdio.h>
#include <stdlib.h>

#include "listmanager.h"
#include "shutdownmanager.h"
#include "keyboard.h"
#include "sender.h"
#include "receiver.h"
#include "screen.h"

/*
    Main Thread
    Simple-talk chat with connecting remote server and ports
    4 threads to handle keyboard inputs, sender, receiever, and screen output
*/

int main (int arc, char** args) {

    if (arc != 4) {
        puts("Invalid Arguments\n");
        puts("Sample: s-talk 9001 csil-cpu6 8989\n");
        return 1;
    }

    int inputPort = atoi(args[1]);
    char * ipAddress = args[2];
    char * outputPort = args[3];

    ListManager_create();

    Keyboard_init();
    Sender_init(ipAddress, outputPort);
    Receiver_init(inputPort);
    Screen_init();
    ShutdownManager_init();
    
    ShutdownManager_waitForShutdown();

    return 0;
}