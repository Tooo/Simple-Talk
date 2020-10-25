#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#include "keyboard.h"
#include "sender.h"
#include "receiver.h"
#include "screen.h"

void freeList(void * item) {
    free(item);
}

int main (int arc, char** args) {

    if (arc != 4) {
        printf("Invalid Arguments\n");
        printf("Sample: s-talk 9001 csil-cpu6.csil.sfuca 8989\n");
        return 1;
    }

    for (int i = 0; i < arc; i++) {
        printf("%s\n", args[i]);
    }

    List * list = List_create();
    Screen_init(list);
    Keyboard_init(list);
    
    Screen_shutdown();
    Keyboard_shutdown();

    List_free(list, freeList);

    return 0;
}