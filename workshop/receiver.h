#ifndef _RECEIVER_H_
#define _RECEIVER_H_

// Start background receive thread
void Receiver_init(char* rxMessage);

// Set dynamic message -- Threadsafe
void Receiver_changeDynamicMessage(char* newDynamic);

// Stop background receive thread and cleanup
void Receiver_shutdown(void);

#endif