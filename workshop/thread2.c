#include <stdio.h>
#include <stdlib.h>
#include "receiver.h"

int main (int arc, char** args) {

    // Start my modules
    Receiver_init("A Message!");

    // Wait for user input
    printf("Enter something to kill thread\n");
    char x;
    scanf("%c", &x);

    // Shutdown my modules
    Receiver_shutdown();

    printf("Done\n");
    return 0;
}