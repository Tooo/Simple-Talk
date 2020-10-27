#ifndef _SENDER_H_
#define _SENDER_H_

void Sender_signalNextMessage();
void Sender_init(char * ipAddress, int portOutput);
void Sender_waitForShutdown();
void Sender_clean();

#endif