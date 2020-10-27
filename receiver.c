#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> // strncmp()
#include <unistd.h>  // close()

#include "receiver.h"
#include "list.h"
#include "listmanager.h"

static pthread_t thread;

static int port;
static int socketDescriptor;
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

    // CHECK ERRORS

    // Bind socket to the port we specified
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

    // CHECK RETURN VALUES

    while (1) {
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MAX_LEN]; // Buffer Rx
        recvfrom(socketDescriptor, messageRx, MAX_LEN, 0, (struct sockaddr *) &sinRemote, &sin_len);

    }
    
    // Close socket
    close(socketDescriptor);
    return NULL;
}

void Receiver_init(int portInput) {
    port = portInput;
    pthread_create(&thread, NULL, receiveThread, NULL);
}

void Receiver_waitForShutdown(void) {
    pthread_join(thread, NULL);
}

void Reciever_clean(void) {
    close(socketDescriptor);

}