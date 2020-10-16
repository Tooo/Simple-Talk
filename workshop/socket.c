#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h> // strncmp()
#include <unistd.h>  // close()

#define MAX_LEN 1024
#define PORT 22110

int main() {

    printf("Net Listen Test on UDP port %d:\n", PORT);
    printf("Connect using: \n");
    printf(" netcat -u IP %d\n", PORT);

    //Address
    struct sockaddr_in sin; //_in means internet
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET; // Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY); // Host to Network long
    sin.sin_port = htons(PORT); // Host to Network short

    // Create socket for UDP
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // Bind socket to the port we specified
    bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

    while (1) {
        // Get data (blocking)
        // Will change sin to be address of client
        // sin passes info in and out of call
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        char messageRx[MAX_LEN]; // Buffer Rx
        int bytesRx = recvfrom(socketDescriptor, messageRx, MAX_LEN, 0, (struct sockaddr *) &sinRemote, &sin_len);

        // Null terminated (string)
        int terminateIdx = (bytesRx < MAX_LEN) ? bytesRx : MAX_LEN - 1;
        messageRx[terminateIdx] = 0;
        printf("Message received (%d bytes): '%s'\n", bytesRx, messageRx);

        // Extract value from message
        // proccess msg any way app requires
        int incMe = atoi(messageRx);
        
        // Create Reply
        // Watch for buffer overflow
        char messageTx[MAX_LEN];
        sprintf(messageTx, "Math: %d + 1 = %d\n", incMe, incMe + 1);

        // Send Message
        sin_len = sizeof(sinRemote);
        sendto(socketDescriptor, messageTx, strlen(messageTx), 0, (struct sockaddr *) &sinRemote, sin_len);
    }
    
    // Close socket
    close(socketDescriptor);
}


