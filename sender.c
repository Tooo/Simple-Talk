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
#include "shutdownmanager.h"

static pthread_t thread;
static pthread_cond_t senderCondVar = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t senderMutex = PTHREAD_MUTEX_INITIALIZER;

static char * port;
static char * ip;
static int socketDescriptor;

static char * message = NULL;
static List * inputList;

void* sendThread(void * unused) {
    struct addrinfo * addr = NULL;
    getaddrinfo(ip, port, 0, &addr);

    // Create socket for UDP
    socketDescriptor = socket(addr->ai_family, addr->ai_socktype, 0);

    // CHECK RETURN VALUES

    while (!ShutdownManager_isShuttingDown()) {
        
        pthread_mutex_lock(&senderMutex);
        {
            pthread_cond_wait(&senderCondVar, &senderMutex);
        }
        pthread_mutex_unlock(&senderMutex);

        ListManager_lockInputList();
        message = List_trim(inputList);
        ListManager_lockInputList();
        
        sendto(socketDescriptor, message, strlen(message), 0, addr->ai_addr, addr->ai_addrlen);

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

void Sender_init(char * ipAddress, char * portOutput) {
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