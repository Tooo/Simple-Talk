#include <stdio.h>
#include "sleep.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> // strncmp()
#include <unistd.h>  // close()

#include "receiver.h"

#define DYNAMIC_LEN 128
#define MAX_LEN 1024
#define PORT 22110

static pthread_mutex_t dynamicMsgMutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_t threadPID;
static int socketDescriptor;
static char * s_rxMessage;
static char* dynamicMessage;

void* receiveThread(void * unused) {

    pthread_mutex_lock(&dynamicMsgMutex);
    {
        strcpy(dynamicMessage, "Dynamic!");
    }
    pthread_mutex_unlock(&dynamicMsgMutex);

    //Address
    struct sockaddr_in sin;                     //_in means internet
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;                   // Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
    sin.sin_port = htons(PORT);                 // Host to Network short

    // Create socket for UDP
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // CHECK ERRORS

    // Bind socket to the port we specified
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

    // CHECK RETURN VALUES

    while (1) {
        // Get data (blocking)
        // Will change sin to be address of client
        // sin passes info in and out of call
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MAX_LEN]; // Buffer Rx
        recvfrom(socketDescriptor, messageRx, MAX_LEN, 0, (struct sockaddr *) &sinRemote, &sin_len);
        
        pthread_mutex_lock(&dynamicMsgMutex);
        {
            printf("%s >> %s: %s", dynamicMessage, s_rxMessage, messageRx);
        }
        pthread_mutex_unlock(&dynamicMsgMutex);

        
    }
    
    // Close socket
    close(socketDescriptor);
    return NULL;
}

void Receiver_init(char* rxMessage) {
    dynamicMessage = malloc(DYNAMIC_LEN);

    s_rxMessage = rxMessage;
    pthread_create(
        &threadPID,         // PID (by pointer)
        NULL,               // Attributes
        receiveThread,      // Functions
        NULL);              // Arguments
}

void Receiver_changeDynamicMessage(char* newDynamic) {
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        strncpy(dynamicMessage, newDynamic, DYNAMIC_LEN);
    }
    pthread_mutex_unlock(&dynamicMsgMutex);
}

void Receiver_shutdown(void) {
    // Cancel thread
    pthread_cancel(threadPID);

    // waits for thread to finish
    pthread_join(threadPID, NULL);

    // Cleanup memory
    pthread_mutex_lock(&dynamicMsgMutex);
    {
        free(dynamicMessage);
    }
    pthread_mutex_unlock(&dynamicMsgMutex);
}