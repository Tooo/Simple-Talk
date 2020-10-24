#ifndef _PRINTER_H_
#define _PRINTER_H_

#include <pthread.h>

//void Printer_init(pthread_mutex_t *psyncOkToPrintMutex, pthread_cond_t *psyncOkToPrintCondVar);
void Printer_init();
void Printer_signalNextChar();
void Printer_waitForShutdown();

#endif