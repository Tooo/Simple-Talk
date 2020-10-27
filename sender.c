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
#include "shutdownManager.h"

static pthread_t thread;
static pthread_cond_t senderCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t senderMutex = PTHREAD_MUTEX_INITIALIZER;

static int port;
static char * ip;
static int socketDescriptor;

static char * message = NULL;
static List * inputList;

void* sendThread(void * unused) {

    //Address
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = ip;
    sin.sin_port = htons(port);

    // Create socket for UDP
    socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // CHECK ERRORS

    // Bind socket to the port we specified
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

    // CHECK RETURN VALUES

    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);

    while (!ShutdownManager_isShuttingDown) {
        pthread_mutex_lock(&senderMutex);
        {
            pthread_cond_wait(&senderCondVar, &senderMutex);
        }
        pthread_mutex_unlock(&senderMutex);

        ListManager_lockInputList();
        message = List_trim(inputList);
        ListManager_lockInputList();
        
        sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr *) &sinRemote, sin_len);

        if (strlen(message) == 2 && message[0] == '!') {
            break;
        }
    }
    return NULL;
}

void Sender_signalNextMessage() {
    pthread_mutex_lock(&senderMutex);
    {
        pthread_cond_signal(&senderCondVar);
    }
    pthread_mutex_unlock(&senderMutex);
}

void Sender_init(char * ipAddress, int portOutput) {
    inputList = ListManager_getInputList();
    ip = ipAddress;
    port = portOutput;
    pthread_create(&thread, NULL, sendThread, NULL);
}

void Sender_waitForShutdown() {
    pthread_join(thread, NULL);
}

void Sender_clean() {
    free(message);
    close(socketDescriptor);
}