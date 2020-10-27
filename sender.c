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
static struct addrinfo * addr = NULL;

static char * message = NULL;
static List * inputList;

void* sendThread(void * unused) {
    
    static struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(ip, port, 0, &addr)) {
        puts("Sender: Failed to getAddrInfo");
        ShutdownManager_triggerShutdown();
        return NULL;
    }

    for (struct addrinfo * i = addr; i != NULL; i = i->ai_next) {
        if ((socketDescriptor = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) != -1) {
            break;
        }
    }
    
    if (!socketDescriptor) {
        puts("Sender: Failed to connect to socket");
        ShutdownManager_triggerShutdown();
        return NULL;
    }

    // CHECK RETURN VALUES

    while (!ShutdownManager_isShuttingDown()) {
        puts("In sender loop");
        pthread_mutex_lock(&senderMutex);
        {
            pthread_cond_wait(&senderCondVar, &senderMutex);
        }
        pthread_mutex_unlock(&senderMutex);

        if (ShutdownManager_isShuttingDown()) {
            break;
        }

        ListManager_lockInputList();
        message = List_trim(inputList);
        ListManager_unlockInputList();

        sendto(socketDescriptor, message, strlen(message), 0, addr->ai_addr, addr->ai_addrlen);
        
        free(message);
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
    pthread_cancel(thread);
    if (!message) {
        free(message);
    }

    if (!addr) {
        free(addr);
    }
    close(socketDescriptor);
}