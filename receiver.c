#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> // strncmp()
#include <unistd.h>  // close()

#include "receiver.h"
#include "screen.h"
#include "list.h"
#include "listmanager.h"
#include "shutdownmanager.h"

static pthread_t thread;

static int port;
static int socketDescriptor;

static char * message = NULL;
static List * outputList;

void* receiveThread(void * unused) {

    //Address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(port);

    // Create socket for UDP
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    if (socketDescriptor == -1) {
        puts("Receiver: Failed to connect to socket");
        ShutdownManager_triggerShutdown();
        return NULL;
    }

    // Bind socket to the port we specified

    if (bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin)) == -1) {
        puts("Receiver: Failed to bind");
        ShutdownManager_triggerShutdown();
        return NULL;
    }

    // CHECK RETURN VALUES

    while (!ShutdownManager_isShuttingDown()) {
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);

        message = malloc(MAX_STRING_LEN);

        if (recvfrom(socketDescriptor, message, MAX_STRING_LEN, 0, (struct sockaddr *) &sinRemote, &sin_len) == -1) {
            puts("Receiver: Failed to recvfrom");
            ShutdownManager_triggerShutdown();
            return NULL;
        }
        ListManager_lockOutputList();
        List_prepend(outputList, message);
        ListManager_unlockOutputList();

        Screen_signalNextMessage();

        if (strlen(message) == 2 && message[0] == '!') {
            ShutdownManager_triggerShutdown();
            break;
        }
    }
    return NULL;
}

void Receiver_init(int portInput) {
    outputList = ListManager_getOutputList();
    port = portInput;
    pthread_create(&thread, NULL, receiveThread, NULL);
}

void Receiver_waitForShutdown() {
    pthread_join(thread, NULL);
}

void Reciever_clean() {
    pthread_cancel(thread);
    if (message) {
        free(message);
    }
    close(socketDescriptor);
}